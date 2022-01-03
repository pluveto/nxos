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

#define LOG_LEVEL LOG_INFO
#include <Utils/Log.h>
#include <XBook/Debug.h>
#include <IO/IRQ.h>

#include <Sched/Sched.h>
#include <Sched/Thread.h>
#include <Sched/MultiCore.h>
#include <Sched/Context.h>

IMPORT ThreadManager ThreadManagerObject;

PUBLIC void SchedToFirstThread(void)
{
    UArch coreId = MultiCoreGetId();
    Thread *thread = MultiCoreDeququeThreadIrqDisabled(coreId);
    ASSERT(thread != NULL);
    ASSERT(MultiCoreSetRunning(coreId, thread) == OS_EOK);
    LOG_D("Sched to first thread:%s/%d", thread->name, thread->tid);
    ContextSwitchNext((Addr)&thread->stack);
    /* should never be here */
    PANIC("Sched to first thread failed!");
}

PRIVATE void PullOrPushThread(UArch coreId)
{
    Thread *thread;
    CoreLocalStorage *cls = CLS_GetIndex(coreId);
    IArch coreThreadCount = AtomicGet(&cls->threadCount);

    /**
     * Adding 1 is to allow the processor core to do more pull operations instead of push operations.
     * Can avoid the threads of the pending queue not running.
     */
    UArch threadsPerCore = AtomicGet(&ThreadManagerObject.activeThreadCount) / NR_MULTI_CORES + 1;

    LOG_D("core#%d: core threads:%d", coreId, coreThreadCount);
    LOG_D("core#%d: active threads:%d", coreId, AtomicGet(&ThreadManagerObject.activeThreadCount));
    LOG_D("core#%d: pending threads:%d", coreId, AtomicGet(&ThreadManagerObject.pendingThreadCount));
    LOG_D("core#%d: threads per core:%d", coreId, threadsPerCore);

    if (coreThreadCount < threadsPerCore)
    {
        /* pull from pending */
        thread = ThreadDequeuePendingList();
        if (thread != NULL)
        {
            LOG_D("---> core#%d: pull thread:%s/%d", coreId, thread->name, thread->tid);
            thread->onCore = coreId;
            ThreadReadyRunLocked(thread, SCHED_HEAD);
        }
    }

    if (coreThreadCount > threadsPerCore)
    {
        /* push to pending */
        thread = MultiCoreDeququeNoAffinityThread(coreId);
        if (thread != NULL)
        {
            LOG_D("---> core#%d: push thread:%s/%d", coreId, thread->name, thread->tid);
            thread->onCore = NR_MULTI_CORES;
            ThreadEnqueuePendingList(thread);
        }
    }
}

/**
 * NOTE: must disable interrupt before call this!
 */
PUBLIC void SchedWithInterruptDisabled(UArch irqLevel)
{
    Thread *next, *prev;
    UArch coreId = MultiCoreGetId();

    /* put prev into list */
    prev = CurrentThread;

    if (prev->state == THREAD_EXIT)
    {
        prev = NULL;    /* not save prev context */
    }
    
    /* pull thread from pending list or push thread to pending list */
    PullOrPushThread(coreId);

    /* get next from local list */
    next = MultiCoreDeququeThreadIrqDisabled(coreId);
    MultiCoreSetRunning(coreId, next);

    if (prev != NULL)
    {
        ASSERT(prev && next);
        LOG_D("CPU#%d Sched prev: %s/%d next: %s/%d", MultiCoreGetId(), prev->name, prev->tid, next->name, next->tid);
        ContextSwitchPrevNext((Addr)&prev->stack, (Addr)&next->stack);
    }
    else
    {
        ContextSwitchNext((Addr)&next->stack);
    }
    INTR_RestoreLevel(irqLevel);
}

PUBLIC void SchedYield(void)
{
    UArch level = INTR_SaveLevel();

    Thread *cur = CurrentThread;
    
    ThreadReadyRunUnlocked(cur, SCHED_TAIL);

    SchedWithInterruptDisabled(level);
}

PUBLIC void SchedExit(void)
{
    UArch level = INTR_SaveLevel();

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
        UArch level = INTR_SaveLevel();
        thread->needSched = 0;

        /* reset ticks from timeslice */
        thread->ticks = thread->timeslice;

        ThreadReadyRunUnlocked(thread, SCHED_TAIL);

        SchedWithInterruptDisabled(level);
    }
    INTR_Disable();
}
