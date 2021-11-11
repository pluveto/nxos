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

#include <Mods/UTest/UTest.h>
#include <Sched/Thread.h>
#include <Sched/Sched.h>

PUBLIC int OS_Main(void)
{
    /* platfrom init */
    if (PlatformInit() != OS_EOK)
    {
        PANIC("Platfrom init failed!" Endln);
    }
    
    LOG_I("Hello, NXOS!");

    InitThread();
    SchedToFirstThread();
    while (1);
    
    HAL_InterruptEnable();
    
    UTestCallInit();

    while (1);
    // SPIN("OS_Main");
    return 0;
}
