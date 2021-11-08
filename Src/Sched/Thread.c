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

#include <Sched/Thread.h>
#include <Sched/ThreadID.h>
#include <MM/Alloc.h>
#include <Utils/String.h>

PUBLIC OS_Error ThreadInit(Thread *thread, const char *name, ThreadHandler handler, void *arg)
{
    ListInit(&thread->list);
    StrCopy(thread->name, name);
    thread->tid = 0;
    thread->handler = handler;
    thread->threadArg = arg;
}

PUBLIC Thread *ThreadCreate(const char *name, ThreadHandler handler, void *arg)
{
    Thread *thread = (Thread *)MemAlloc(sizeof(Thread));
    if (thread == NULL)
    {
        return NULL;
    }
    ThreadInit(thread, name, handler, arg);
    return thread;
}

PUBLIC InitThread(void)
{
    InitThreadID();
}
