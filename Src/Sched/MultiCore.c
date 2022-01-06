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
#define NX_LOG_NAME "Core"
#include <Utils/Log.h>

#include <XBook/Debug.h>

NX_PUBLIC NX_STATIC_ATOMIC_INIT(NX_ActivedCoreCount, 0);

/* init as zero, avoid cleared by clear bss action */
NX_PRIVATE NX_VOLATILE NX_UArch BootCoreId = 0;

NX_PRIVATE NX_CoreLocalStorage CLS[NX_MULTI_CORES_NR];

NX_PUBLIC void NX_MultiCorePreload(NX_UArch coreId)
{
    /* recored boot core */
    BootCoreId = coreId;
}

NX_PUBLIC NX_UArch NX_MultiCoreGetBootCore(void)
{
    return BootCoreId;
}

/**
 * init multi core before thread
 */
NX_PUBLIC void NX_MultiCoreInit(NX_UArch coreId)
{
    /* init core array */
    int i;
    for (i = 0; i < NX_MULTI_CORES_NR; i++)
    {
        CLS[i].threadRunning = NX_NULL;
        NX_ListInit(&CLS[i].threadReadyList);
        NX_SpinInit(&CLS[i].lock);
        NX_AtomicSet(&CLS[i].threadCount, 0);
    }
}

/**
 * get CLS by index
 */
NX_PUBLIC NX_CoreLocalStorage *CLS_GetIndex(NX_UArch coreId)
{
    NX_ASSERT(coreId < NX_MULTI_CORES_NR);
    return &CLS[coreId];
}

NX_PUBLIC void NX_MultiCoreMain(NX_UArch coreId)
{
    /* boot other cores */
    if (NX_MultiCoreBootApp(coreId) != NX_EOK)
    {
        NX_LOG_W("Boot multi core failed!");
    }
    else
    {
        NX_LOG_I("Boot multi core success!");
    }
}

NX_PUBLIC void NX_MultiCoreStage2(NX_UArch appCoreId)
{
    // NX_LOG_I("core %d stage2", appCoreId);
    NX_Error err;
    err = NX_MultiCoreEnterApp(appCoreId);
    if (err != NX_EOK)
    {
        NX_LOG_E("app core: %d setup failed!", appCoreId);
    }
    else
    {
        NX_LOG_I("app core: %d setup success!", appCoreId);
    }
}

NX_PUBLIC void NX_MultiCoreEnqueueThreadIrqDisabled(NX_UArch coreId, NX_Thread *thread, int flags)
{
    NX_CoreLocalStorage *cls = CLS_GetIndex(coreId);

    NX_SpinLock(&cls->lock, NX_True);

    if (flags & NX_SCHED_HEAD)
    {
        NX_ListAdd(&thread->list, &cls->threadReadyList);
    }
    else
    {
        NX_ListAddTail(&thread->list, &cls->threadReadyList);
    }

    NX_AtomicInc(&cls->threadCount);

    NX_SpinUnlock(&cls->lock);
}

NX_PUBLIC NX_Thread *NX_MultiCoreDeququeThreadIrqDisabled(NX_UArch coreId)
{
    NX_Thread *thread;
    NX_CoreLocalStorage *cls = CLS_GetIndex(coreId);
    
    NX_SpinLock(&cls->lock, NX_True);
    
    thread = NX_ListFirstEntry(&cls->threadReadyList, NX_Thread, list);
    NX_ListDel(&thread->list);

    NX_AtomicDec(&cls->threadCount);

    NX_SpinUnlock(&cls->lock);

    return thread;
}

/**
 * NOTE: this must called irq disabled
 */
NX_PUBLIC NX_Thread *NX_MultiCoreDeququeNoAffinityThread(NX_UArch coreId)
{
    NX_Thread *thread, *findThread = NX_NULL;
    NX_CoreLocalStorage *cls = CLS_GetIndex(coreId);
    
    NX_SpinLock(&cls->lock, NX_True);
    
    NX_ListForEachEntry(thread, &cls->threadReadyList, list)
    {
        if (thread->coreAffinity >= NX_MULTI_CORES_NR) /* no affinity on any core */
        {
            findThread = thread;
            NX_ListDel(&thread->list);
            NX_AtomicDec(&cls->threadCount);
            break;
        }
    }
    
    NX_SpinUnlock(&cls->lock);

    return findThread;
}

NX_PUBLIC NX_Error NX_MultiCoreSetRunning(NX_UArch coreId, NX_Thread *thread)
{
    if (coreId >= NX_MULTI_CORES_NR || thread == NX_NULL)
    {
        return NX_EINVAL;
    }

    NX_CoreLocalStorage *cls = CLS_GetIndex(coreId);
    NX_UArch level;
    NX_SpinLockIRQ(&cls->lock, &level);
    thread->state = NX_THREAD_RUNNING;
    cls->threadRunning = thread;
    NX_SpinUnlockIRQ(&cls->lock, level);
    return NX_EOK;
}

/**
 * get running thread
 */
NX_PUBLIC NX_Thread *CLS_GetRunning(void)
{
    NX_Thread *thread;
    NX_CoreLocalStorage *cls = CLS_Get();
    NX_UArch level;
    NX_SpinLockIRQ(&cls->lock, &level);
    thread = cls->threadRunning;
    NX_SpinUnlockIRQ(&cls->lock, level);
    return thread;
}
