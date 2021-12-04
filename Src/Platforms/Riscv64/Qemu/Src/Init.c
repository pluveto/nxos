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

#define LOG_LEVEL LOG_INFO
#define LOG_NAME "INIT"
#include <Utils/Debug.h>

PUBLIC void ClearBSS(void);

INTERFACE OS_Error PlatformInit(void)
{
    ClearBSS();
    LOG_I("Hello, QEMU Riscv64!");
    CPU_InitTrap();
    HAL_InitClock();
    
    PageInit();
    
    return OS_EOK;
}

INTERFACE OS_Error PlatformStage2(void)
{
    LOG_I("stage2!");
    
    INTR_Enable();
    
    while (1);
    CASM ("ebreak");
    return OS_EOK;
}
