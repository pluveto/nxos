/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Init OS 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-3      JasonHu           Init
 */

#define LOG_NAME "OS Main"
#include <Utils/Debug.h>

#include <Mods/Test/UTest.h>
#include <Sched/Thread.h>
#include <Sched/Sched.h>
#include <Sched/MultiCore.h>
#include <MM/HeapCache.h>
#include <MM/PageHeap.h>
#include <IO/IRQ.h>
#include <Mods/Time/Timer.h>

/* Platform init */
INTERFACE OS_Error PlatformInit(UArch coreId);

/**
 * stage2 means you can do:
 * 1. use MemAlloc/MemFree
 * 2. use Bind IRQ
 * 3. use Thread
 * 4. use Timer
 */
INTERFACE WEAK_SYM OS_Error PlatformStage2(void)
{
    return OS_EOK;
}

/* used to record how many core entered main */
PRIVATE STATIC_ATOMIC_INIT(MainEnterReference);

PUBLIC int OS_Main(UArch coreId)
{
    if (!AtomicGet(&MainEnterReference))
    {
        AtomicInc(&MainEnterReference);
        
        /* init multi core before enter platform */
        MultiCorePreload(coreId);
        
        /* platfrom init */
        if (PlatformInit(coreId) != OS_EOK)
        {
            PANIC("Platfrom init failed!" Endln);
        }
        LOG_I("Hello, NXOS!");

        /* init irq */
        IRQ_Init();

        /* init page heap */
        PageHeapInit();
        
        /* init heap cache for MemAlloc & MemFree */
        HeapCacheInit();
        
        /* init timer */
        TimersInit();

        /* init multi core */
        MultiCoreInit(coreId);

        /* init thread */
        ThreadsInit();
        
        if (ClockInit() != OS_EOK)
        {
            PANIC("Clock init failed!" Endln);
        }
        
        /* init auto calls */
        CallsInit();
        
        /* platform stage2 call */
        if (PlatformStage2() != OS_EOK)
        {
            PANIC("Platform stage2 failed!");
        }
        
        MultiCoreMain(coreId);
    }
    else
    {
        MultiCoreStage2(coreId);
    }
    /* start sched */
    SchedToFirstThread();
    /* should never be here */
    PANIC("should never be here!");
    return 0;
}
