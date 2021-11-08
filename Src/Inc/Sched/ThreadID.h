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

// #define MAX_THREAD_ID_NR 0xffff
#define MAX_THREAD_ID_NR 32

struct ThreadID
{
    U32 *maps;
    U32 nextID;
};

PUBLIC I32 ThreadIdAlloc(void);
PUBLIC void ThreadIdFree(I32 id);
PUBLIC void InitThreadID(void);

#endif /* __SCHED_THREAD_ID__ */
