/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Thread ID management
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-7      JasonHu           Init
 */

#ifndef __SCHED_THREAD_ID__
#define __SCHED_THREAD_ID__

#include <XBook.h>
#include <Sched/Mutex.h>

#ifdef CONFIG_MAX_THREAD_NR
#define MAX_THREAD_NR CONFIG_MAX_THREAD_NR
#else
#define MAX_THREAD_NR 64
#endif

struct ThreadID
{
    U32 *maps;
    U32 nextID;
    Mutex idLock;
};

PUBLIC int ThreadIdAlloc(void);
PUBLIC void ThreadIdFree(int id);
PUBLIC void ThreadsInitID(void);

#endif /* __SCHED_THREAD_ID__ */
