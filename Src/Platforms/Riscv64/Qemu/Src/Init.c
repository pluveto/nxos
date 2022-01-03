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
#include <SBI.h>
#include <Regs.h>
#include <Drivers/DirectUart.h>
#include <Sched/MultiCore.h>
#include <Utils/Log.h>

#define LOG_LEVEL LOG_INFO
#define LOG_NAME "INIT"
#include <XBook/Debug.h>

PUBLIC void ClearBSS(void);

INTERFACE OS_Error PlatformInit(UArch coreId)
{
    ClearBSS();

    /* NOTE: after inited trap, you can use core id func */
    CPU_InitTrap(coreId);

    HAL_DirectUartInit();

    sbi_init();
    sbi_print_version();

    LOG_I("Hello, QEMU Riscv64!");
    
    HAL_InitClock();
    PLIC_Init(TRUE);
    
    PageInit();
    
    return OS_EOK;
}

INTERFACE OS_Error PlatformStage2(void)
{
    LOG_I("stage2!");
    HAL_DirectUartStage2();
    // INTR_Enable();
    // while (1);
    return OS_EOK;
}
