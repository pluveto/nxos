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
    HAL_ContextSwitchNext((Addr)&thread->stack);
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
    if (prev->state == THREAD_RUNNING || prev->state == THREAD_READY)
    {
        if (prev->state == THREAD_RUNNING)
        {
            prev->ticks = prev->timeslice;  /* reset ticks */
            prev->state = THREAD_READY;
        }
        ListAddTail(&prev->list, &threadReadyList);
    }
    else if (prev->state == THREAD_EXIT)
    {
        prev = NULL;    /* set null */
        /* need free prev thread and stack */
        // LOG_D("Thread Exit");
    }
    else
    {
        /* error state */
        PANIC("Thread sched with errnor state");
    }
    
    /* get next from list */
    next = ListFirstEntry(&threadReadyList, Thread, list);
    ListDel(&next->list);
    next->state = THREAD_RUNNING;

    /* set current */
    currentThread = next;

    if (prev)
    {
        // LOG_D("Sched prev:" $d(prev->tid) " next:" $d(next->tid));
        HAL_ContextSwitchPrevNext((Addr)&prev->stack, (Addr)&next->stack);
    }
    else
    {
        HAL_ContextSwitchNext((Addr)&next->stack);
    }
    
    HAL_InterruptEnable();
}

PUBLIC void SchedYield(void)
{
    HAL_InterruptDisable();
    currentThread->state = THREAD_READY;
    SchedWithInterruptDisabled();
}

PUBLIC void SchedExit(void)
{
    HAL_InterruptDisable();
    currentThread->state = THREAD_EXIT;
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
