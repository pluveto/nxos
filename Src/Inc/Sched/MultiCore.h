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

struct CoreLocalStorage
{
    List threadReadyList;   /* list for thread ready to run */
    Thread *threadRunning;  /* the thread running on core */

    Spin lock;     /* lock for CLS */
    Atomic threadCount;    /* ready thread count on this core */
};
typedef struct CoreLocalStorage CoreLocalStorage;

struct MultiCoreOps
{
    UArch (*getId)(void);
    OS_Error (*bootApp)(UArch bootCoreId);
    OS_Error (*enterApp)(UArch appCoreId);
};

INTERFACE IMPORT struct MultiCoreOps MultiCoreOpsInterface; 

#define MultiCoreBootApp MultiCoreOpsInterface.bootApp
#define MultiCoreEnterApp MultiCoreOpsInterface.enterApp
#define MultiCoreGetId MultiCoreOpsInterface.getId

PUBLIC void MultiCorePreload(UArch coreId);
PUBLIC void MultiCoreInit(UArch coreId);
PUBLIC void MultiCoreMain(UArch coreId);
PUBLIC void MultiCoreStage2(UArch appCoreId);

PUBLIC UArch MultiCoreGetBootCore(void);

PUBLIC void MultiCoreEnqueueThreadIrqDisabled(UArch coreId, Thread *thread, int flags);
PUBLIC Thread *MultiCoreDeququeThreadIrqDisabled(UArch coreId);
PUBLIC OS_Error MultiCoreSetRunning(UArch coreId, Thread *thread);

PUBLIC CoreLocalStorage *CLS_GetIndex(UArch coreId);

PUBLIC Thread *MultiCoreDeququeNoAffinityThread(UArch coreId);

/**
 * get CLS by core id
 */
INLINE CoreLocalStorage *CLS_Get(void)
{
    return CLS_GetIndex(MultiCoreGetId());
}

PUBLIC Thread *CLS_GetRunning(void);

#endif /* __SCHED_MULTI_CORE___ */
