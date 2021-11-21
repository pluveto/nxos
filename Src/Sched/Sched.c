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

IMPORT List threadReadyList;
IMPORT List exitThreadList;

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
PUBLIC void SchedWithInterruptDisabled(Uint irqLevel)
{
    Thread *next, *prev;

    /* put prev into list */
    prev = currentThread;

    if (prev->state == THREAD_EXIT)
    {
        prev = NULL;    /* not save prev context */
    }
    
    /* get next from list */
    next = ListFirstEntry(&threadReadyList, Thread, list);
    ListDel(&next->list);
    next->state = THREAD_RUNNING;

    /* set current */
    currentThread = next;

    if (prev)
    {
        // LOG_D("Sched prev: %d next: %d", prev->tid, next->tid);
        // LOG_D("Sched prev: %s next: %s", prev->name, next->name);
        HAL_ContextSwitchPrevNext((Addr)&prev->stack, (Addr)&next->stack);
    }
    else
    {
        HAL_ContextSwitchNext((Addr)&next->stack);
    }
    
    HAL_InterruptRestoreLevel(irqLevel);
}

PUBLIC void SchedYield(void)
{
    Uint level = HAL_InterruptSaveLevel();
    /* put thread to tail of ready list */
    currentThread->state = THREAD_READY;
    ListAddTail(&currentThread->list, &threadReadyList);
    SchedWithInterruptDisabled(level);
}

PUBLIC void SchedExit(void)
{
    Uint level = HAL_InterruptSaveLevel();

    /* set exit state, del from global list and add to exit list */
    currentThread->state = THREAD_EXIT;
    LOG_D("Thread exit: %d", currentThread->tid);
    ListAdd(&currentThread->globalList, &exitThreadList);
    SchedWithInterruptDisabled(level);
}

PUBLIC void ReSchedCheck(void)
{
    Thread *thread = currentThread;
    if (thread->isTerminated)
    {
        LOG_D("call terminate: %d", thread->tid);
        thread->isTerminated = 0;
        ThreadExit();
    }
    if (thread->needSched)
    {
        Uint level = HAL_InterruptSaveLevel();
        thread->needSched = 0;

        /* put thread to tail of thread and reset ticks from timeslice */
        thread->ticks = thread->timeslice;
        thread->state = THREAD_READY;
        ListAddTail(&thread->list, &threadReadyList);

        SchedWithInterruptDisabled(level);
    }
}
