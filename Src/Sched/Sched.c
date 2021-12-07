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
#include <XBook/HAL.h>

IMPORT List ThreadReadyList;
IMPORT List ThreadExitList;
IMPORT Thread *CurrentThread;

PUBLIC void SchedToFirstThread(void)
{
    /* get a thread from ready list */
    Thread *thread = ListFirstEntry(&ThreadReadyList, Thread, list);
    ListDel(&thread->list);
    thread->state = THREAD_RUNNING;
    CurrentThread = thread;
    LOG_D("Sched to first thread:%s/%d", thread->name, thread->tid);
    HAL_ContextSwitchNext((Addr)&thread->stack);
    /* should never be here */
    PANIC("Sched to first thread failed!");
}

/**
 * NOTE: must disable interrupt before call this!
 */
PUBLIC void SchedWithInterruptDisabled(Uint irqLevel)
{
    Thread *next, *prev;

    /* put prev into list */
    prev = CurrentThread;

    if (prev->state == THREAD_EXIT)
    {
        prev = NULL;    /* not save prev context */
    }
    
    /* get next from list */
    next = ListFirstEntry(&ThreadReadyList, Thread, list);
    ListDel(&next->list);
    next->state = THREAD_RUNNING;

    /* set current */
    CurrentThread = next;

    if (prev != NULL)
    {
        ASSERT(prev && next);
        //LOG_D("Sched prev: %s/%d next: %s/%d", prev->name, prev->tid, next->name, next->tid);
        HAL_ContextSwitchPrevNext((Addr)&prev->stack, (Addr)&next->stack);
    }
    else
    {
        HAL_ContextSwitchNext((Addr)&next->stack);
    }
    INTR_RestoreLevel(irqLevel);
}

PUBLIC void SchedYield(void)
{
    Uint level = INTR_SaveLevel();
    /* put thread to tail of ready list */
    CurrentThread->state = THREAD_READY;
    ListAddTail(&CurrentThread->list, &ThreadReadyList);
    SchedWithInterruptDisabled(level);
}

PUBLIC void SchedExit(void)
{
    Uint level = INTR_SaveLevel();

    /* set exit state, del from global list and add to exit list */
    CurrentThread->state = THREAD_EXIT;
    LOG_D("Thread exit: %d", CurrentThread->tid);
    ListAdd(&CurrentThread->globalList, &ThreadExitList);
    SchedWithInterruptDisabled(level);
}

PUBLIC void ReSchedCheck(void)
{
    INTR_Enable();

    Thread *thread = CurrentThread;
    if (thread->isTerminated)
    {
        LOG_D("call terminate: %d", thread->tid);
        thread->isTerminated = 0;
        ThreadExit();
    }
    if (thread->needSched)
    {
        Uint level = INTR_SaveLevel();
        thread->needSched = 0;

        /* put thread to tail of thread and reset ticks from timeslice */
        thread->ticks = thread->timeslice;
        thread->state = THREAD_READY;
        ListAddTail(&thread->list, &ThreadReadyList);

        SchedWithInterruptDisabled(level);
    }
    INTR_Disable();
}
