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
#include <MM/Alloc.h>
#include <Utils/String.h>

/* TODO: add lock lock for thread list */
PUBLIC List globalThreadList;
PUBLIC List exitThreadList;
PUBLIC List threadReadyList;
PUBLIC Thread *currentThread = NULL;

PUBLIC Thread *ThreadFindById(U32 tid)
{
    Thread *thread;
    ListForEachEntry (thread, &globalThreadList, globalList)
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
    ListAdd(&thread->list, &threadReadyList);
    ListAdd(&thread->globalList, &globalThreadList);
    return OS_EOK;
}

PUBLIC OS_Error ThreadTerminate(Thread *thread)
{
    if (thread == NULL)
    {
        return OS_EINVAL;
    }
    Uint level = HAL_InterruptSaveLevel();
    thread->isTerminated = 1;
    HAL_InterruptRestoreLevel(level);
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
    return currentThread;
}


/**
 * system idle thread on per cpu.
 */
PRIVATE void IdleThread(void *arg)
{
    LOG_I("Hello, idle thread\n");
    int i = 0;
    while (1)
    {
        // LOG_I("Counter: " $d(i));
        i++;
        SchedYield();
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
        HAL_InterruptDisable();
        ListForEachEntrySafe (thread, safe, &exitThreadList, globalList)
        {
            LOG_D("daemon release thread:" $d(thread->tid));
            /* del from exit list */
            ListDel(&thread->globalList);

            ThreadReleaseSelf(thread);
        }
        HAL_InterruptEnable();

        /* do delay or timeout */
        SchedYield();
    }
    
}

PRIVATE void TestThread1(void *arg)
{
    LOG_I("Hello, test thread 1: " $p(arg) "\n");
    // Thread *self = ThreadSelf();
    int i = 0;
    while (1)
    {
        // LOG_I("Thread: " $(self->name) " tid: " $d(self->tid) ".");
        i++;
        if (i > 100)
        {
            ThreadExit();
        }
    }
}

PRIVATE void TestThread2(void *arg)
{
    LOG_I("Hello, test thread 2: " $p(arg) "\n");
    
    Thread *self = ThreadSelf();
    int i = 0;
    while (1)
    {
        i++;
        // LOG_I("Thread: " $(self->name) " tid: " $d(self->tid) ".");
        if (i > 100)
        {
            break;
        }
    }
    LOG_I("thread exit: " $s(self->name));
}

PRIVATE void TestThread3(void *arg)
{
    LOG_I("Hello, test thread 3: " $p(arg) "\n");
    
    /* wait terminate */
    while (1)
    {
        
    }
}

PRIVATE U32 thread3ID;

PRIVATE void TestThread4(void *arg)
{
    LOG_I("Hello, test thread 4: " $p(arg) "\n");
    Thread *target = ThreadFindById(thread3ID);
    ASSERT(target != NULL);
    int i = 0;
    while (1)
    {
        i++;
        // LOG_I("Thread: " $(self->name) " tid: " $d(self->tid) ".");
        if (i == 100)
        {
            LOG_D("terminate thread:" $d(target->tid));
            ThreadTerminate(target);
        }
        if (i == 1000)
        {
            return;
        }
    }
}

PRIVATE void TestThread(void)
{
    Thread *thread = ThreadCreate("test thread 1", TestThread1, (void *) 0x1234abcd);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);

    thread = ThreadCreate("test thread 2", TestThread2, (void *) 0x1234abcd);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);

    thread = ThreadCreate("test thread 3", TestThread3, (void *) 0x1234abcd);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);

    thread3ID = thread->tid;
    
    thread = ThreadCreate("test thread 4", TestThread4, (void *) 0x1234abcd);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);

    thread = ThreadCreate("test thread 3", TestThread2, (void *) 0x1234abcd);
    ASSERT(thread != NULL);
    ASSERT(ThreadDestroy(thread) == OS_EOK);

    LOG_D("thread test done.");    
}

PUBLIC void InitThread(void)
{
    InitThreadID();
    ListInit(&globalThreadList);
    ListInit(&exitThreadList);
    ListInit(&threadReadyList);
    currentThread = NULL;

    /* init idle thread */
    Thread *thread = ThreadCreate("idleN", IdleThread, NULL);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);

    /* init daemon thread */
    thread = ThreadCreate("daemon", DaemonThread, NULL);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);
    
    TestThread();
}
