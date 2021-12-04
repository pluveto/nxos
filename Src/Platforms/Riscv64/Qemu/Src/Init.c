/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Init Riscv64 Qemu platfrom 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

#include <XBook.h>
#include <Trap.h>
#include <Clock.h>
#include <Page.h>
#include <Platform.h>
#include <PLIC.h>

#define LOG_LEVEL LOG_INFO
#define LOG_NAME "INIT"
#include <Utils/Debug.h>

PUBLIC void ClearBSS(void);

INTERFACE OS_Error PlatformInit(void)
{
    ClearBSS();
    LOG_I("Hello, QEMU Riscv64!");
    if (NR_CPUS <= 0 || NR_CPUS > PLATFORM_MAX_NR_CPUS)
    {
        PANIC("config CPU number error!");    
    }

    CPU_InitTrap();
    HAL_InitClock();
    PLIC_Init();
    
    PageInit();
    
    return OS_EOK;
}

INTERFACE OS_Error PlatformStage2(void)
{
    LOG_I("stage2!");
    HAL_DirectUartStage2();

    INTR_Enable();

    PlatformMain();

    while (1);
    return OS_EOK;
}
