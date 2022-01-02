/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Thread for NXOS
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-7      JasonHu           Init
 */

#define LOG_NAME "Thread"
#include <Utils/Debug.h>

#include <Sched/Thread.h>
#include <Sched/ThreadID.h>
#include <Sched/Sched.h>
#include <Sched/Mutex.h>
#include <Sched/MultiCore.h>
#include <MM/Alloc.h>
#include <Utils/String.h>
#include <Mods/Time/Timer.h>

PUBLIC ThreadManager ThreadManagerObject;

PRIVATE OS_Error ThreadInit(Thread *thread, 
    const char *name,
    ThreadHandler handler, void *arg,
    U8 *stack, Size stackSize)
{
    if (thread == NULL || name == NULL || handler == NULL || stack == NULL || !stackSize)
    {
        return OS_EINVAL;
    }

    ListInit(&thread->list);
    ListInit(&thread->globalList);
    StrCopy(thread->name, name);
    thread->tid = ThreadIdAlloc();
    if (thread->tid < 0)
    {
        return OS_ENORES;
    }
    thread->state = THREAD_INIT;
    thread->handler = handler;
    thread->threadArg = arg;
    thread->timeslice = 3;
    thread->ticks = thread->timeslice;
    thread->needSched = 0;
    thread->isTerminated = 0;
    thread->stackBase = stack;
    thread->stackSize = stackSize;
    thread->stack = thread->stackBase + stackSize - sizeof(Uint);
    thread->stack = HAL_ContextInit(handler, arg, thread->stack, (void *)ThreadExit);
    
    thread->onCore = NR_MULTI_CORES; /* not on any core */
    thread->coreAffinity = NR_MULTI_CORES; /* no core affinity */

    thread->resource.sleepTimer = NULL;

    SpinInit(&thread->lock);
    return OS_EOK;
}

PRIVATE OS_Error ThreadDeInit(Thread *thread)
{
    if (thread == NULL)
    {
        return OS_EINVAL;
    }
    if (thread->stackBase == NULL)
    {
        return OS_EFAULT;
    }
    ThreadIdFree(thread->tid);    
    return OS_EOK;
}

PUBLIC Thread *ThreadCreate(const char *name, ThreadHandler handler, void *arg)
{
    Thread *thread = (Thread *)MemAlloc(sizeof(Thread));
    if (thread == NULL)
    {
        return NULL;
    }
    U8 *stack = MemAlloc(THREAD_STACK_SIZE_DEFAULT);
    if (stack == NULL)
    {
        MemFree(thread);
        return NULL;
    }
    if (ThreadInit(thread, name, handler, arg, stack, THREAD_STACK_SIZE_DEFAULT) != OS_EOK)
    {
        MemFree(stack);
        MemFree(thread);
        return NULL;
    }
    return thread;
}

PUBLIC OS_Error ThreadDestroy(Thread *thread)
{
    if (thread == NULL)
    {
        return OS_EINVAL;
    }
    U8 *stackBase = thread->stackBase;
    if (stackBase == NULL)
    {
        return OS_EFAULT;
    }

    OS_Error err = ThreadDeInit(thread);
    if (err != OS_EOK)
    {
        return err;
    }

    MemFree(stackBase);

    MemFree(thread);
    return OS_EOK;
}

PUBLIC void ThreadReadyRunLocked(Thread *thread, int flags)
{
    thread->state = THREAD_READY;

    if (thread->onCore < NR_MULTI_CORES)
    {
        MultiCoreEnqueueThreadIrqDisabled(thread->onCore, thread, flags);
    }
    else
    {
        if (flags & SCHED_HEAD)
        {
            ListAdd(&thread->list, &ThreadManagerObject.pendingList);
        }
        else
        {
            ListAddTail(&thread->list, &ThreadManagerObject.pendingList);
        }
        AtomicInc(&ThreadManagerObject.pendingThreadCount);
    }
}

PUBLIC void ThreadReadyRunUnlocked(Thread *thread, int flags)
{
    Uint level;
    SpinLockIRQ(&ThreadManagerObject.lock, &level);

    ThreadReadyRunLocked(thread, flags);
    
    SpinUnlockIRQ(&ThreadManagerObject.lock, level);
}

INLINE void ThreadEnququeGlobalListUnlocked(Thread *thread)
{
    ListAdd(&thread->globalList, &ThreadManagerObject.globalList);    
    AtomicInc(&ThreadManagerObject.activeThreadCount);
}

INLINE void ThreadDeququeGlobalListUnlocked(Thread *thread)
{
    ListDel(&thread->globalList);
    AtomicDec(&ThreadManagerObject.activeThreadCount);
}

PUBLIC OS_Error ThreadRun(Thread *thread)
{
    if (thread == NULL)
    {
        return OS_EINVAL;
    }

    Uint level;
    SpinLockIRQ(&ThreadManagerObject.lock, &level);

    ThreadEnququeGlobalListUnlocked(thread);

    /* add to ready list */
    ThreadReadyRunLocked(thread, SCHED_TAIL);
    
    SpinUnlockIRQ(&ThreadManagerObject.lock, level);
    return OS_EOK;
}

PUBLIC void ThreadYield(void)
{
    SchedYield();
}

PUBLIC OS_Error ThreadTerminate(Thread *thread)
{
    if (thread == NULL)
    {
        return OS_EINVAL;
    }
    if (thread->state == THREAD_INIT || thread->state == THREAD_EXIT)
    {
        return OS_EPERM;
    }

    Uint level = INTR_SaveLevel();
    thread->isTerminated = 1;
    ThreadWakeup(thread);
    INTR_RestoreLevel(level);
    return OS_EOK;
}

/**
 * release resource the thread hold.
 */
PRIVATE void ThreadReleaseResouce(Thread *thread)
{
    /* free tid */
    ThreadIdFree(thread->tid);

    /* NOTE: add other resource here. */
    if (thread->resource.sleepTimer != NULL)
    {
        TimerStop(thread->resource.sleepTimer);
        thread->resource.sleepTimer = NULL;
    }
}

/**
 * release resouce must for a thread run
 */
PRIVATE void ThreadReleaseSelf(Thread *thread)
{
    /* free stack */
    MemFree(thread->stackBase);
    /* free thread struct */
    MemFree(thread);
}

PUBLIC void ThreadExit(void)
{
    /* free thread resource */
    Thread *thread = ThreadSelf();

    /* release the resource here that not the must for a thread! */
    ThreadReleaseResouce(thread);

    Uint level;
    SpinLockIRQ(&ThreadManagerObject.lock, &level);

    ThreadDeququeGlobalListUnlocked(thread);

    SpinUnlockIRQ(&ThreadManagerObject.lock, level);
    
    SchedExit();
    PANIC("Thread Exit should never arrival here!");
}

PUBLIC Thread *ThreadSelf(void)
{
    Thread *cur = CLS_GetRunning();
    ASSERT(cur != NULL);
    return cur;
}

/**
 * must called when interrupt disabled
 */
PRIVATE void ThreadBlockInterruptDisabled(ThreadState state, Uint irqLevel)
{
    ASSERT(state == THREAD_SLEEP || state == THREAD_DEEPSLEEP);
    CurrentThread->state = state;
    SchedWithInterruptDisabled(irqLevel);
}

/**
 * must called when interrupt disabled
 * quickly: thread can be run quickly
 */
PRIVATE void ThreadUnblockInterruptDisabled(Thread *thread)
{
    ThreadReadyRunLocked(thread, SCHED_HEAD);
}

/**
 * wakeup a thread, must called interrupt disabled
 */
PUBLIC OS_Error ThreadWakeup(Thread *thread)
{
    if (thread == NULL)
    {
        return OS_EINVAL;
    }
    /* if thread in sleep, then wakeup it */
    if (thread->state == THREAD_SLEEP)
    {
        ThreadUnblockInterruptDisabled(thread);
    }
    return OS_EOK;
}

PRIVATE Bool TimerThreadSleepTimeout(Timer *timer, void *arg)
{
    Thread *thread = (Thread *)arg; /* the thread wait for timeout  */

    ASSERT(thread->state == THREAD_SLEEP);

    thread->resource.sleepTimer = NULL; /* cleanup sleep timer */

    if (ThreadWakeup(thread) != OS_EOK)
    {
        LOG_E("Wakeup thread:%s/%d failed!", thread->name, thread->tid);
    }
    else
    {
        LOG_I("Wakeup thread:%s/%d success!", thread->name, thread->tid);
    }
    return TRUE;
}

/* if thread sleep less equal than 2s, use delay instead */
#define THREAD_SLEEP_TIMEOUT_THRESHOLD 2

PUBLIC OS_Error ThreadSleep(Uint microseconds)
{
    if (microseconds == 0)
    {
        return OS_EINVAL;
    }
    if (microseconds <= THREAD_SLEEP_TIMEOUT_THRESHOLD)
    {
        return ClockTickDelayMillisecond(microseconds);
    }

    Thread *self = ThreadSelf();
    Timer sleepTimer;
    OS_Error err;
    err = TimerInit(&sleepTimer, microseconds, TimerThreadSleepTimeout, (void *)self, TIMER_ONESHOT);
    if (err != OS_EOK)
    {
        return err;
    }

    Uint irqLevel = INTR_SaveLevel();
    /* lock thread */
    self->resource.sleepTimer = &sleepTimer;

    TimerStart(&sleepTimer);

    /* set thread as sleep state */
    ThreadBlockInterruptDisabled(THREAD_SLEEP, irqLevel);

    /* if sleep timer always here, it means that the thread was interrupted! */
    if (self->resource.sleepTimer != NULL)
    {
        /* thread was interrupted! */
        TimerStop(self->resource.sleepTimer);
        self->resource.sleepTimer = NULL;
        return OS_EINTR;
    }
    return OS_EOK;
}

PUBLIC OS_Error ThreadSetAffinity(Thread *thread, Uint coreId)
{
    if (thread == NULL || coreId >= NR_MULTI_CORES)
    {
        return OS_EINVAL;
    }
    Uint level;
    SpinLockIRQ(&thread->lock, &level);
    thread->coreAffinity = coreId;
    thread->onCore = coreId;
    SpinUnlockIRQ(&thread->lock, level);
    return OS_EOK;
}

PUBLIC void ThreadEnqueuePendingList(Thread *thread)
{
    Uint level;
    SpinLockIRQ(&ThreadManagerObject.lock, &level);
    ListAdd(&thread->list, &ThreadManagerObject.pendingList);
    AtomicInc(&ThreadManagerObject.pendingThreadCount);
    SpinUnlockIRQ(&ThreadManagerObject.lock, level);
}

PUBLIC Thread *ThreadDequeuePendingList(void)
{
    Thread *thread;
    SpinLock(&ThreadManagerObject.lock, TRUE);
    thread = ListFirstEntryOrNULL(&ThreadManagerObject.pendingList, Thread, list);
    if (thread != NULL)
    {
        ListDel(&thread->list);
        AtomicDec(&ThreadManagerObject.pendingThreadCount);
    }
    SpinUnlock(&ThreadManagerObject.lock);
    return thread;
}

PUBLIC void ThreadEnququeExitList(Thread *thread)
{
    Uint level;
    SpinLockIRQ(&ThreadManagerObject.exitLock, &level);
    ListAdd(&thread->globalList, &ThreadManagerObject.exitList);
    SpinUnlockIRQ(&ThreadManagerObject.exitLock, level);
}

PUBLIC Thread *ThreadFindById(U32 tid)
{
    Thread *thread = NULL, *find = NULL;
    Uint level;

    SpinLockIRQ(&ThreadManagerObject.lock, &level);

    ListForEachEntry (thread, &ThreadManagerObject.globalList, globalList)
    {
        if (thread->tid == tid)
        {
            find = thread;
            break;
        }
    }

    SpinUnlockIRQ(&ThreadManagerObject.lock, level);
    return find;
}

/**
 * system idle thread on per cpu.
 */
PRIVATE void IdleThreadEntry(void *arg)
{
    LOG_I("Idle thread: %s startting...", ThreadSelf()->name);
    int i = 0;
    while (1)
    {
        i++;
        ThreadYield();
    }
}

/**
 * system deamon thread for all cpus 
 */
PRIVATE void DaemonThreadEntry(void *arg)
{
    LOG_I("Daemon thread started.\n");
    Thread *thread, *safe;
    Uint level;
    while (1)
    {
        SpinLockIRQ(&ThreadManagerObject.exitLock, &level);
        ListForEachEntrySafe (thread, safe, &ThreadManagerObject.exitList, globalList)
        {
            LOG_D("---> daemon release thread: %s/%d", thread->name, thread->tid);
            /* del from exit list */
            ListDel(&thread->globalList);
            SpinUnlockIRQ(&ThreadManagerObject.exitLock, level);
            ThreadReleaseSelf(thread);
            SpinLockIRQ(&ThreadManagerObject.exitLock, &level);
        }
        SpinUnlockIRQ(&ThreadManagerObject.exitLock, level);

        /* do delay or timeout, sleep 0.5s */
        ThreadYield();
    }
}

PUBLIC void ThreadManagerInit(void)
{
    AtomicSet(&ThreadManagerObject.averageThreadThreshold, 0);
    AtomicSet(&ThreadManagerObject.activeThreadCount, 0);
    AtomicSet(&ThreadManagerObject.pendingThreadCount, 0);
    ListInit(&ThreadManagerObject.exitList);
    ListInit(&ThreadManagerObject.globalList);
    ListInit(&ThreadManagerObject.pendingList);
    
    SpinInit(&ThreadManagerObject.lock);
    SpinInit(&ThreadManagerObject.exitLock);
}

PUBLIC void ThreadsInit(void)
{
    Thread *idleThread;
    Thread *deamonThread;
    int coreId;
    char name[8];
    ThreadsInitID();
    ThreadManagerInit();

    /* init idle thread */
    for (coreId = 0; coreId < NR_MULTI_CORES; coreId++)
    {
        StrPrintfN(name, 8, "Idle%d", coreId);
        idleThread = ThreadCreate(name, IdleThreadEntry, NULL);
        ASSERT(idleThread != NULL);
        /* bind idle on each core */
        ThreadSetAffinity(idleThread, coreId);

        ASSERT(ThreadRun(idleThread) == OS_EOK);
    }

    /* init daemon thread */
    deamonThread = ThreadCreate("daemon", DaemonThreadEntry, NULL);
    ASSERT(deamonThread != NULL);
    ASSERT(ThreadRun(deamonThread) == OS_EOK);
}
