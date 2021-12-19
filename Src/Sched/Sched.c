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
#include <Sched/MultiCore.h>

IMPORT ThreadManager ThreadManagerObject;

PUBLIC void SchedToFirstThread(void)
{
    Uint coreId = MultiCoreGetId();
    Thread *thread = MultiCoreDeququeThreadIrqDisabled(coreId);
    ASSERT(thread != NULL);
    ASSERT(MultiCoreSetRunning(coreId, thread) == OS_EOK);
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
    Thread *next, *prev, *thread;
    Uint coreId = MultiCoreGetId();

    /* put prev into list */
    prev = CurrentThread;

    if (prev->state == THREAD_EXIT)
    {
        prev = NULL;    /* not save prev context */
    }
    
    /* pull thread from pending list */
    thread = ThreadDequeuePendingList();
    if (thread != NULL)
    {
        thread->onCore = coreId;
        ThreadReadyRunLocked(thread, SCHED_HEAD);
    }

    /* get next from local list */
    next = MultiCoreDeququeThreadIrqDisabled(coreId);
    MultiCoreSetRunning(coreId, next);

    if (prev != NULL)
    {
        ASSERT(prev && next);
        //LOG_D("Sched prev: %s/%d next: %s/%d", prev->name, prev->tid, next->name, next->tid);
        //LOG_D("CPU#%d Sched", MultiCoreGetId());
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

    Thread *cur = CurrentThread;
    
    ThreadReadyRunUnlocked(cur, SCHED_TAIL);

    SchedWithInterruptDisabled(level);
}

PUBLIC void SchedExit(void)
{
    Uint level = INTR_SaveLevel();

    Thread *cur = CurrentThread;
    LOG_D("Thread exit: %d", cur->tid);

    cur->state = THREAD_EXIT;
    ThreadEnququeExitList(cur);

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

        /* reset ticks from timeslice */
        thread->ticks = thread->timeslice;

        ThreadReadyRunUnlocked(thread, SCHED_TAIL);

        SchedWithInterruptDisabled(level);
    }
    INTR_Disable();
}
