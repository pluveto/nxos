/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Scheduler for NXOS
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-8      JasonHu           Init
 */

#include <Sched/Sched.h>
#include <Sched/Thread.h>
#include <HAL.h>

IMPORT List globalThreadList;
IMPORT List threadReadyList;

PUBLIC void SchedToFirstThread(void)
{
    /* get a thread from ready list */
    Thread *thread = ListFirstEntry(&threadReadyList, Thread, list);
    ListDel(&thread->list);
    currentThread = thread;
    HAL_ContextSwitchNext((Addr)&thread->stackTop);
}
