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

#include <XBook/HAL.h>
#define LOG_NAME "Main"
#define LOG_LEVEL LOG_INFO
#include <Utils/Debug.h>

#include <Mods/Test/UTest.h>
#include <Sched/Thread.h>
#include <Sched/Sched.h>
#include <MM/HeapCache.h>
#include <MM/PageHeap.h>
#include <Mods/Time/Timer.h>

PUBLIC int OS_Main(void)
{
    /* platfrom init */
    if (PlatformInit() != OS_EOK)
    {
        PANIC("Platfrom init failed!" Endln);
    }
    LOG_I("Hello, NXOS!");

    /* init page heap */
    PageHeapInit();
    
    /* init heap cache for MemAlloc & MemFree */
    HeapCacheInit();
    
    /* init timer */
    TimersInit();

    /* init thread */
    ThreadsInit();
    
    /* init auto calls */
    CallsInit();
    
    /* start sched */
    SchedToFirstThread();
    PANIC("should never be here!");
    return 0;
}
