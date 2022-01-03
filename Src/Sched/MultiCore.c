/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: mutil core sched
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-10     JasonHu           Init
 */

#include <Sched/MultiCore.h>
#include <Sched/Thread.h>
#include <Sched/Sched.h>
#define LOG_NAME "Core"
#include <Utils/Log.h>

#include <XBook/Debug.h>

/* init as zero, avoid cleared by clear bss action */
PRIVATE VOLATILE UArch BootCoreId = 0;

PRIVATE CoreLocalStorage CLS[NR_MULTI_CORES];

PUBLIC void MultiCorePreload(UArch coreId)
{
    /* recored boot core */
    BootCoreId = coreId;
}

PUBLIC UArch MultiCoreGetBootCore(void)
{
    return BootCoreId;
}

/**
 * init multi core before thread
 */
PUBLIC void MultiCoreInit(UArch coreId)
{
    /* init core array */
    int i;
    for (i = 0; i < NR_MULTI_CORES; i++)
    {
        CLS[i].threadRunning = NULL;
        ListInit(&CLS[i].threadReadyList);
        SpinInit(&CLS[i].lock);
        AtomicSet(&CLS[i].threadCount, 0);
    }
}

/**
 * get CLS by index
 */
PUBLIC CoreLocalStorage *CLS_GetIndex(UArch coreId)
{
    ASSERT(coreId < NR_MULTI_CORES);
    return &CLS[coreId];
}

PUBLIC void MultiCoreMain(UArch coreId)
{
    /* boot other cores */
    if (MultiCoreBootApp(coreId) != OS_EOK)
    {
        LOG_W("Boot multi core failed!");
    }
    else
    {
        LOG_I("Boot multi core success!");
    }
}

PUBLIC void MultiCoreStage2(UArch appCoreId)
{
    // LOG_I("core %d stage2", appCoreId);
    OS_Error err;
    err = MultiCoreEnterApp(appCoreId);
    if (err != OS_EOK)
    {
        LOG_E("app core: %d setup failed!", appCoreId);
    }
    else
    {
        LOG_I("app core: %d setup success!", appCoreId);
    }
}

PUBLIC void MultiCoreEnqueueThreadIrqDisabled(UArch coreId, Thread *thread, int flags)
{
    CoreLocalStorage *cls = CLS_GetIndex(coreId);

    SpinLock(&cls->lock, TRUE);

    if (flags & SCHED_HEAD)
    {
        ListAdd(&thread->list, &cls->threadReadyList);
    }
    else
    {
        ListAddTail(&thread->list, &cls->threadReadyList);
    }

    AtomicInc(&cls->threadCount);

    SpinUnlock(&cls->lock);
}

PUBLIC Thread *MultiCoreDeququeThreadIrqDisabled(UArch coreId)
{
    Thread *thread;
    CoreLocalStorage *cls = CLS_GetIndex(coreId);
    
    SpinLock(&cls->lock, TRUE);
    
    thread = ListFirstEntry(&cls->threadReadyList, Thread, list);
    ListDel(&thread->list);

    AtomicDec(&cls->threadCount);

    SpinUnlock(&cls->lock);

    return thread;
}

/**
 * NOTE: this must called irq disabled
 */
PUBLIC Thread *MultiCoreDeququeNoAffinityThread(UArch coreId)
{
    Thread *thread, *findThread = NULL;
    CoreLocalStorage *cls = CLS_GetIndex(coreId);
    
    SpinLock(&cls->lock, TRUE);
    
    ListForEachEntry(thread, &cls->threadReadyList, list)
    {
        if (thread->coreAffinity >= NR_MULTI_CORES) /* no affinity on any core */
        {
            findThread = thread;
            ListDel(&thread->list);
            AtomicDec(&cls->threadCount);
            break;
        }
    }
    
    SpinUnlock(&cls->lock);

    return findThread;
}

PUBLIC OS_Error MultiCoreSetRunning(UArch coreId, Thread *thread)
{
    if (coreId >= NR_MULTI_CORES || thread == NULL)
    {
        return OS_EINVAL;
    }

    CoreLocalStorage *cls = CLS_GetIndex(coreId);
    SpinLockIRQ(&cls->lock);
    thread->state = THREAD_RUNNING;
    cls->threadRunning = thread;
    SpinUnlockIRQ(&cls->lock);
    return OS_EOK;
}

/**
 * get running thread
 */
PUBLIC Thread *CLS_GetRunning(void)
{
    Thread *thread;
    CoreLocalStorage *cls = CLS_Get();
    SpinLockIRQ(&cls->lock);
    thread = cls->threadRunning;
    SpinUnlockIRQ(&cls->lock);
    return thread;
}
