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

#include <Utils/Debug.h>

/* init as zero, avoid cleared by clear bss action */
PRIVATE VOLATILE Uint BootCoreId = 0;

PRIVATE CoreLocalStorage CLS[NR_MULTI_CORES];

PUBLIC void MultiCorePreload(Uint coreId)
{
    /* recored boot core */
    BootCoreId = coreId;
}

PUBLIC Uint MultiCoreGetBootCore(void)
{
    return BootCoreId;
}

/**
 * init multi core before thread
 */
PUBLIC void MultiCoreInit(Uint coreId)
{
    /* init core array */
    int i;
    for (i = 0; i < NR_MULTI_CORES; i++)
    {
        CLS[i].threadRunning = NULL;
        ListInit(&CLS[i].threadReadyList);
        SpinInit(&CLS[i].lock);
        AtomicSet(&CLS[i].readyThreadCount, 0);
    }
}

/**
 * get CLS by index
 */
PUBLIC CoreLocalStorage *CLS_GetIndex(Uint coreId)
{
    ASSERT(coreId < NR_MULTI_CORES);
    return &CLS[coreId];
}

PUBLIC void MultiCoreMain(Uint coreId)
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

PUBLIC void MultiCoreStage2(Uint appCoreId)
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

PUBLIC void MultiCoreEnqueueThreadIrqDisabled(Uint coreId, Thread *thread, int flags)
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

    SpinUnlock(&cls->lock);
}

PUBLIC Thread *MultiCoreDeququeThreadIrqDisabled(Uint coreId)
{
    Thread *thread;
    CoreLocalStorage *cls = CLS_GetIndex(coreId);
    
    SpinLock(&cls->lock, TRUE);
    
    thread = ListFirstEntry(&cls->threadReadyList, Thread, list);
    ListDel(&thread->list);

    SpinUnlock(&cls->lock);

    return thread;
}

PUBLIC OS_Error MultiCoreSetRunning(Uint coreId, Thread *thread)
{
    if (coreId >= NR_MULTI_CORES || thread == NULL)
    {
        return OS_EINVAL;
    }

    CoreLocalStorage *cls = CLS_GetIndex(coreId);
    Uint level;
    SpinLockIRQ(&cls->lock, &level);
    thread->state = THREAD_RUNNING;
    cls->threadRunning = thread;
    SpinUnlockIRQ(&cls->lock, level);
    return OS_EOK;
}

/**
 * get running thread
 */
PUBLIC Thread *CLS_GetRunning(void)
{
    Thread *thread;
    CoreLocalStorage *cls = CLS_Get();
    Uint level;
    SpinLockIRQ(&cls->lock, &level);
    thread = cls->threadRunning;
    SpinUnlockIRQ(&cls->lock, level);
    return thread;
}
