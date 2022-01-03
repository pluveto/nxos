/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: HAL Multi Core support
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-9      JasonHu           Init
 */

#include <XBook.h>
#include <Sched/MultiCore.h>
#include <MM/Barrier.h>
#include <Platform.h>
#define LOG_NAME "Multi Core"
#include <Utils/Log.h>

#include <SBI.h>
#include <Trap.h>
#include <Clock.h>
#include <PLIC.h>
#include <Regs.h>

IMPORT Addr TrapEntry0;
IMPORT Addr TrapEntry1;
IMPORT Addr TrapEntry2;
IMPORT Addr TrapEntry3;

/**
 * Within SBI, we can't read mhartid, so I try to use `trap entry` to see who am I.
 */
PRIVATE UArch HAL_CoreGetId(void)
{
    Addr trapEntry = ReadCSR(stvec);

    if (trapEntry == (Addr)&TrapEntry0)
    {
        return 0;
    }
    else if (trapEntry == (Addr)&TrapEntry1)
    {
        return 1;
    }
    else if (trapEntry == (Addr)&TrapEntry2)
    {
        return 2;
    }
    else if (trapEntry == (Addr)&TrapEntry3)
    {
        return 3;
    }
    /* should never be here */
    while (1);
}

PRIVATE OS_Error HAL_CoreBootApp(UArch bootCoreId)
{
    LOG_I("boot core is:%d", bootCoreId);
    UArch coreId;
    for (coreId = 0; coreId < NR_MULTI_CORES; coreId++)
    {
        LOG_I("core#%d state:%d", coreId, sbi_hsm_hart_status(coreId));
        if (bootCoreId == coreId) /* skip boot core */
        {
            LOG_I("core#%d is boot core, skip it", coreId);
            continue;
        }
        LOG_I("wakeup app core:%d", coreId);
        sbi_hsm_hart_start(coreId, MEM_KERNEL_BASE, 0);    
        MemoryBarrier();
        LOG_I("core#%d state:%d after wakeup", coreId, sbi_hsm_hart_status(coreId));
    }
    return OS_EOK;
}

PRIVATE OS_Error HAL_CoreEnterApp(UArch appCoreId)
{
    /* NOTE: init trap first before do anything */
    CPU_InitTrap(appCoreId);
    LOG_I("core#%d enter application!", appCoreId);
    HAL_InitClock();
    PLIC_Init(FALSE);

    return OS_EOK;
    // return OS_EOK;
    /* don't sched now */
    // INTR_Enable();
    while (1)
    {
        CASM("wfi");
    }    
    return OS_EOK;
}

INTERFACE struct MultiCoreOps MultiCoreOpsInterface = 
{
    .getId = HAL_CoreGetId,
    .bootApp = HAL_CoreBootApp,
    .enterApp = HAL_CoreEnterApp,
};
