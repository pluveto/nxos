/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Thread test
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-13     JasonHu           Init
 */

#define LOG_NAME "Thread"
#include <Utils/Debug.h>

#include <Sched/Thread.h>

#ifdef CONFIG_THREAD_TEST
PRIVATE void TestThread1(void *arg)
{
    LOG_I("Hello, test thread 1: %p", arg);
    // Thread *self = ThreadSelf();
    int i = 0;
    while (1)
    {
        i++;
        if (i > 100)
        {
            ThreadExit();
        }
    }
}

PRIVATE void TestThread2(void *arg)
{
    LOG_I("Hello, test thread 2: %p", arg);
    
    Thread *self = ThreadSelf();
    int i = 0;
    while (1)
    {
        i++;
        if (i > 100)
        {
            break;
        }
    }
    LOG_I("thread exit: " $s(self->name));
}

PRIVATE void TestThread3(void *arg)
{
    LOG_I("Hello, test thread 3: %p", arg);
    
    /* wait terminate */
    while (1)
    {
        
    }
}

PRIVATE U32 thread3ID;

PRIVATE void TestThread4(void *arg)
{
    LOG_I("Hello, test thread 4: %p", arg);
    Thread *target = ThreadFindById(thread3ID);
    ASSERT(target != NULL);
    int i = 0;
    while (1)
    {
        i++;
        if (i == 100)
        {
            LOG_D("terminate thread: %d", target->tid);
            ThreadTerminate(target);
        }
        if (i == 1000)
        {
            return;
        }
    }
}
#endif /* CONFIG_THREAD_TEST */


#ifdef CONFIG_MUTEX_TEST

#include <Sched/Mutex.h>

PRIVATE Mutex mutexLock;

PRIVATE void TestMutex1(void *arg)
{
    int i = 0;
    while (1)
    {
        i++;
        if (i > 100)
        {
            return;
        }
        MutexLock(&mutexLock, TRUE);
        LOG_I(ThreadSelf()->name, " get lock");
        MutexUnlock(&mutexLock);
        ClockTickDelayMilliSecond(10);
    }
}

#endif /* CONFIG_MUTEX_TEST */


PUBLIC void TestThread(void)
{
#ifdef CONFIG_THREAD_TEST
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
#endif /* CONFIG_THREAD_TEST */

#ifdef CONFIG_MUTEX_TEST
    MutexInit(&mutexLock);

    Thread *mutexThread = ThreadCreate("mutex thread 1", TestMutex1, NULL);
    ASSERT(mutexThread != NULL);
    ASSERT(ThreadRun(mutexThread) == OS_EOK);

    mutexThread = ThreadCreate("mutex thread 2", TestMutex1, NULL);
    ASSERT(mutexThread != NULL);
    ASSERT(ThreadRun(mutexThread) == OS_EOK);
    
    mutexThread = ThreadCreate("mutex thread 3", TestMutex1, NULL);
    ASSERT(mutexThread != NULL);
    ASSERT(ThreadRun(mutexThread) == OS_EOK);
    
#endif /* CONFIG_MUTEX_TEST */

    LOG_D("thread test done.");    
}
