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

#ifndef __SCHED_THREAD__
#define __SCHED_THREAD__

#include <Utils/List.h>
#include <Mods/Time/Timer.h>
#include <Sched/Spin.h>

#ifdef CONFIG_THREAD_NAME_LEN
#define THREAD_NAME_LEN CONFIG_THREAD_NAME_LEN
#else
#define THREAD_NAME_LEN 32
#endif

#ifdef CONFIG_THREAD_STACK_SIZE
#define THREAD_STACK_SIZE_DEFAULT CONFIG_THREAD_STACK_SIZE
#else
#define THREAD_STACK_SIZE_DEFAULT 8192
#endif

typedef void (*ThreadHandler)(void *arg);

enum ThreadState
{
    THREAD_INIT,
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_SLEEP,
    THREAD_DEEPSLEEP,
    THREAD_EXIT,
};
typedef enum ThreadState ThreadState;

struct ThreadResource
{
    Timer *sleepTimer;
};
typedef struct ThreadResource ThreadResource;

struct Thread
{
    /* thread list */
    List list;
    List globalList;

    Spin lock;  /* lock for thread */

    /* thread info */
    ThreadState state;
    U32 tid;     /* thread id */
    ThreadHandler handler;
    void *threadArg;
    char name[THREAD_NAME_LEN];
    
    /* thread stack */
    U8 *stackBase;  /* stack base */
    USize stackSize; 
    U8 *stack;      /* stack top */
    
    /* thread sched */
    U32 timeslice;
    U32 ticks;
    U32 needSched;
    U32 isTerminated;
    UArch onCore;        /* thread on which core */
    UArch coreAffinity;  /* thread would like to run on the core */

    /* thread resource */
    ThreadResource resource;
};
typedef struct Thread Thread;

struct ThreadManager
{
    List globalList;    /* for global thread list */
    List exitList;      /* for thread will exit soon */
    List pendingList;   /* for thread ready to be pulled by the core */
    Atomic averageThreadThreshold;    /* Average number of threads on core for load balance */
    Atomic activeThreadCount;
    Atomic pendingThreadCount;

    Spin lock;    /* lock for thread manager */
    Spin exitLock;    /* lock for thread exit */
};
typedef struct ThreadManager ThreadManager;

#define CurrentThread ThreadSelf()

PUBLIC Thread *ThreadCreate(const char *name, ThreadHandler handler, void *arg);
PUBLIC OS_Error ThreadDestroy(Thread *thread);

PUBLIC OS_Error ThreadTerminate(Thread *thread);
PUBLIC void ThreadExit(void);
PUBLIC Thread *ThreadSelf(void);
PUBLIC Thread *ThreadFindById(U32 tid);

PUBLIC OS_Error ThreadRun(Thread *thread);
PUBLIC void ThreadYield(void);
PUBLIC OS_Error ThreadSetAffinity(Thread *thread, UArch coreId);

PUBLIC OS_Error ThreadSleep(UArch microseconds);
PUBLIC OS_Error ThreadWakeup(Thread *thread);

PUBLIC void ThreadsInit(void);
PUBLIC void TestThread(void);

PUBLIC void SchedToFirstThread(void);

PUBLIC void ThreadEnququeExitList(Thread *thread);
PUBLIC void ThreadEnqueuePendingList(Thread *thread);
PUBLIC Thread *ThreadDequeuePendingList(void);

PUBLIC void ThreadReadyRunLocked(Thread *thread, int flags);
PUBLIC void ThreadReadyRunUnlocked(Thread *thread, int flags);

#endif /* __SCHED_THREAD__ */
