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
#include <PageZone.h>
#include <Platform.h>
#include <PLIC.h>
#include <SBI.h>
#include <Regs.h>
#include <Drivers/DirectUart.h>
#include <Sched/SMP.h>
#include <Utils/Log.h>

#define NX_LOG_LEVEL NX_LOG_INFO
#define NX_LOG_NAME "INIT"
#include <XBook/Debug.h>

NX_PUBLIC void HAL_ClearBSS(void);

NX_INTERFACE NX_Error HAL_PlatformInit(NX_UArch coreId)
{
    HAL_ClearBSS();

    /* NOTE: init trap first before do anything */
    CPU_InitTrap(coreId);

    HAL_DirectUartInit();

    sbi_init();
    sbi_print_version();

    NX_LOG_I("Hello, QEMU Riscv64!");
    
    PLIC_Init(NX_True);
    
    HAL_PageZoneInit();
    
    return NX_EOK;
}

NX_INTERFACE NX_Error HAL_PlatformStage2(void)
{
    NX_LOG_I("stage2!");
    HAL_DirectUartStage2();

    return NX_EOK;
}
