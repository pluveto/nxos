/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: multi core sched
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-10     JasonHu           Init
 */

#ifndef __SCHED_MULTI_CORE___
#define __SCHED_MULTI_CORE___

#include <XBook.h>
#include <Utils/List.h>
#include <Sched/Thread.h>
#include <Sched/Spin.h>
#include <XBook/Atomic.h>

struct NX_CoreLocalStorage
{
    NX_List threadReadyList;   /* list for thread ready to run */
    NX_Thread *threadRunning;  /* the thread running on core */

    NX_Spin lock;     /* lock for CLS */
    NX_Atomic threadCount;    /* ready thread count on this core */
};
typedef struct NX_CoreLocalStorage NX_CoreLocalStorage;

struct NX_MultiCoreOps
{
    NX_UArch (*getId)(void);
    NX_Error (*bootApp)(NX_UArch bootCoreId);
    NX_Error (*enterApp)(NX_UArch appCoreId);
};

NX_INTERFACE NX_IMPORT struct NX_MultiCoreOps NX_MultiCoreOpsInterface; 

#define NX_MultiCoreBootApp    NX_MultiCoreOpsInterface.bootApp
#define NX_MultiCoreEnterApp   NX_MultiCoreOpsInterface.enterApp
#define NX_MultiCoreGetId      NX_MultiCoreOpsInterface.getId

NX_PUBLIC void NX_MultiCorePreload(NX_UArch coreId);
NX_PUBLIC void NX_MultiCoreInit(NX_UArch coreId);
NX_PUBLIC void NX_MultiCoreMain(NX_UArch coreId);
NX_PUBLIC void NX_MultiCoreStage2(NX_UArch appCoreId);

NX_PUBLIC NX_UArch NX_MultiCoreGetBootCore(void);

NX_PUBLIC void NX_MultiCoreEnqueueThreadIrqDisabled(NX_UArch coreId, NX_Thread *thread, int flags);
NX_PUBLIC NX_Thread *NX_MultiCoreDeququeThreadIrqDisabled(NX_UArch coreId);
NX_PUBLIC NX_Error NX_MultiCoreSetRunning(NX_UArch coreId, NX_Thread *thread);

NX_PUBLIC NX_CoreLocalStorage *CLS_GetIndex(NX_UArch coreId);

NX_PUBLIC NX_Thread *NX_MultiCoreDeququeNoAffinityThread(NX_UArch coreId);

/**
 * get CLS by core id
 */
NX_INLINE NX_CoreLocalStorage *CLS_Get(void)
{
    return CLS_GetIndex(NX_MultiCoreGetId());
}

NX_PUBLIC NX_Thread *CLS_GetRunning(void);

#endif /* __SCHED_MULTI_CORE___ */
