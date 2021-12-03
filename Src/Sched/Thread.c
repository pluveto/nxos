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
#include <MM/Alloc.h>
#include <Utils/String.h>
#include <Mods/Time/Timer.h>

/* TODO: add lock lock for thread list */
PUBLIC List ThreadGlobalList;
PUBLIC List ThreadExitList;
PUBLIC List ThreadReadyList;
PUBLIC Thread *CurrentThread = NULL;

PUBLIC Thread *ThreadFindById(U32 tid)
{
    Thread *thread;
    ListForEachEntry (thread, &ThreadGlobalList, globalList)
    {
        if (thread->tid == tid)
        {
            return thread;
        }
    }
    return NULL;
}

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
    thread->stackBase = stack;
    thread->stackSize = stackSize;
    thread->stack = thread->stackBase + stackSize;
    thread->stack = HAL_ContextInit(handler, arg, thread->stack, (void *)ThreadExit);
    
    thread->resource.sleepTimer = NULL;
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

PUBLIC OS_Error ThreadRun(Thread *thread)
{
    if (thread == NULL)
    {
        return OS_EINVAL;
    }
    thread->state = THREAD_READY;
    ListAdd(&thread->list, &ThreadReadyList);
    ListAdd(&thread->globalList, &ThreadGlobalList);
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

    /* thread exit from global list */
    ListDel(&thread->globalList);
    
    SchedExit();
    PANIC("Thread Exit should never arrival here!");
}

PUBLIC Thread *ThreadSelf(void)
{
    return CurrentThread;
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
    /* add thread to ready list */
    CurrentThread->state = THREAD_READY;
    /* add to list head so that can be run quickly */
    ListAdd(&thread->list, &ThreadReadyList);    
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

PRIVATE void ThreadSleepTimeout(Timer *timer, void *arg)
{
    Thread *thread = (Thread *)arg; /* the thread wait for timeout  */
    
    if (ThreadWakeup(thread) != OS_EOK)
    {
        LOG_E("Wakeup thread:%s/%d failed!\n", thread->name, thread->tid);
    }
    else
    {
        LOG_I("Wakeup thread:%s/%d success!\n", thread->name, thread->tid);
    }
    
    thread->resource.sleepTimer = NULL; /* cleanup sleep timer */

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
    err = TimerInit(&sleepTimer, microseconds, ThreadSleepTimeout, (void *)self, TIMER_ONESHOT);
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

/**
 * system idle thread on per cpu.
 */
PRIVATE void IdleThread(void *arg)
{
    LOG_I("Hello, idle thread");
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
PRIVATE void DaemonThread(void *arg)
{
    LOG_I("Daemon thread started.\n");
    Thread *thread, *safe;
    while (1)
    {
        /* TODO: lock thread instead interrupt disable */
        INTR_Disable();
        ListForEachEntrySafe (thread, safe, &ThreadExitList, globalList)
        {
            LOG_D("daemon release thread: %s/%d", thread->name, thread->tid);
            /* del from exit list */
            ListDel(&thread->globalList);

            ThreadReleaseSelf(thread);
        }
        INTR_Enable();

        /* do delay or timeout */
        ThreadYield();
    }
}

PUBLIC void ThreadsInit(void)
{
    ThreadsInitID();
    ListInit(&ThreadGlobalList);
    ListInit(&ThreadExitList);
    ListInit(&ThreadReadyList);
    CurrentThread = NULL;

    /* init idle thread */
    Thread *thread = ThreadCreate("idle/N", IdleThread, NULL);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);

    /* init daemon thread */
    thread = ThreadCreate("daemon", DaemonThread, NULL);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);
}
