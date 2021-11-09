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

#include <Utils/Debug.h>

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
    thread->state = THREAD_RUNNING;
    currentThread = thread;
    HAL_ContextSwitchNext((Addr)&thread->stackTop);
}

/**
 * NOTE: must disable interrupt before call this!
 */
PUBLIC void SchedWithInterruptDisabled(void)
{
    Thread *next, *prev;

    /* put prev into list */
    prev = currentThread;

    /* according state to set info */
    if (prev->state == THREAD_RUNNING)
    {
        prev->ticks = prev->timeslice;  /* reset ticks */
        prev->state = THREAD_READY;
        LOG_D("sched running");
    }
    else if (prev->state == THREAD_READY)
    {
        /* do nothing */
        LOG_D("sched yiled");
    }
    else
    {
        /* error state */
        PANIC("Thread sched with errnor state");
    }
    ListAddTail(&prev->list, &threadReadyList);

    /* get next from list */
    next = ListFirstEntry(&threadReadyList, Thread, list);
    ListDel(&next->list);
    next->state = THREAD_RUNNING;

    /* set current */
    currentThread = next;

    // LOG_D("Sched prev:" $d(prev->tid) " next:" $d(next->tid));
    /* do switch */
    HAL_ContextSwitchPrevNext((Addr)&prev->stackTop, (Addr)&next->stackTop);
    
    HAL_InterruptEnable();
}

PUBLIC void SchedYield(void)
{
    HAL_InterruptDisable();
    currentThread->state = THREAD_READY;
    SchedWithInterruptDisabled();
}

PUBLIC void ReSchedCheck(void)
{
    Thread *thread = currentThread;
    if (thread->needSched)
    {
        HAL_InterruptDisable();
        thread->needSched = 0;
        SchedWithInterruptDisabled();
    }
}
