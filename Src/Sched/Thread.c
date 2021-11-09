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
PUBLIC List threadReadyList;
PUBLIC Thread *currentThread = NULL;

PUBLIC OS_Error ThreadInit(Thread *thread, 
    const char *name,
    ThreadHandler handler, void *arg,
    U8 *stack, Size stackSize)
{
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
    thread->stack = stack;
    thread->stackSize = stackSize;
    thread->stackTop = thread->stack + stackSize;
    thread->stackTop = HAL_ContextInit(handler, arg, thread->stackTop, (void *)ThreadExit);
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
        MemFreeSafety(thread);
        return NULL;
    }
    if (ThreadInit(thread, name, handler, arg, stack, THREAD_STACK_SIZE_DEFAULT) != OS_EOK)
    {
        MemFreeSafety(stack);
        MemFreeSafety(thread);
        return NULL;
    }
    return thread;
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

PUBLIC void ThreadExit(void)
{

    PANIC("Thread Exit should never arrival here!");
}

PUBLIC Thread *ThreadSelf(void)
{
    return currentThread;
}

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

PRIVATE void TestThread1(void *arg)
{
    LOG_I("Hello, test thread 1: " $p(arg) "\n");
    Thread *self = ThreadSelf();
    while (1)
    {
        LOG_I("Thread: " $(self->name) " tid: " $d(self->tid) ".");    
    }
}

PRIVATE void TestThread2(void *arg)
{
    LOG_I("Hello, test thread 2: " $p(arg) "\n");
    
    Thread *self = ThreadSelf();
    while (1)
    {
        LOG_I("Thread: " $(self->name) " tid: " $d(self->tid) ".");    
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
}

PUBLIC void InitThread(void)
{
    InitThreadID();
    ListInit(&globalThreadList);
    ListInit(&threadReadyList);
    currentThread = NULL;

    /* init idle thread */
    Thread *thread = ThreadCreate("idleN", IdleThread, NULL);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);

    TestThread();
}
