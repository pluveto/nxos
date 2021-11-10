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

#define THREAD_NAME_LEN 32

#define THREAD_STACK_SIZE_DEFAULT 4096

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

struct Thread
{
    List list;
    List globalList;
    ThreadState state;
    Uint32 tid;     /* thread id */
    U8 *stack;      /* stack base */
    Size stackSize; 
    U8 *stackTop;   /* stack top */
    ThreadHandler handler;
    void *threadArg;
    U32 timeslice;
    U32 ticks;
    U32 needSched;
    char name[THREAD_NAME_LEN];
};
typedef struct Thread Thread;

PUBLIC Thread *currentThread;

PUBLIC OS_Error ThreadInit(Thread *thread, 
    const char *name, 
    ThreadHandler handler, void *arg,
    U8 *stack, Size stackSize);
PUBLIC Thread *ThreadCreate(const char *name, ThreadHandler handler, void *arg);


PUBLIC void ThreadExit(void);
PUBLIC Thread *ThreadSelf(void);

PUBLIC void InitThread(void);

#endif /* __SCHED_THREAD__ */