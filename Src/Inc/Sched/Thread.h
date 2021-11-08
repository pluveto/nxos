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

typedef void (*ThreadHandler)(void *arg);

struct Thread
{
    List list;
    Uint32 tid;     /* thread id */
    ThreadHandler handler;
    void *threadArg;

    char name[THREAD_NAME_LEN];
};
typedef struct Thread Thread;

PUBLIC OS_Error ThreadInit(Thread *thread, const char *name, ThreadHandler handler, void *arg);
PUBLIC Thread *ThreadCreate(const char *name, ThreadHandler handler, void *arg);

PUBLIC InitThread(void);

#endif /* __SCHED_THREAD__ */
