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
#include <Sched/SMP.h>
#include <MM/Barrier.h>
#include <Platform.h>
#define NX_LOG_NAME "Multi Core"
#include <Utils/Log.h>

#include <SBI.h>
#include <Trap.h>
#include <Clock.h>
#include <PLIC.h>
#include <Regs.h>

NX_IMPORT NX_Addr TrapEntry0;
NX_IMPORT NX_Addr TrapEntry1;
NX_IMPORT NX_Addr TrapEntry2;
NX_IMPORT NX_Addr TrapEntry3;

/**
 * Within SBI, we can't read mhartid, so I try to use `trap entry` to see who am I.
 */
NX_PRIVATE NX_UArch HAL_CoreGetIndex(void)
{
    NX_Addr trapEntry = ReadCSR(stvec);

    if (trapEntry == (NX_Addr)&TrapEntry0)
    {
        return 0;
    }
    else if (trapEntry == (NX_Addr)&TrapEntry1)
    {
        return 1;
    }
    else if (trapEntry == (NX_Addr)&TrapEntry2)
    {
        return 2;
    }
    else if (trapEntry == (NX_Addr)&TrapEntry3)
    {
        return 3;
    }
    /* should never be here */
    while (1);
}

NX_PRIVATE NX_Error HAL_CoreBootApp(NX_UArch bootCoreId)
{
    NX_LOG_I("boot core is:%d", bootCoreId);
    NX_UArch coreId;
    for (coreId = 0; coreId < NX_MULTI_CORES_NR; coreId++)
    {
        NX_LOG_I("core#%d state:%d", coreId, sbi_hsm_hart_status(coreId));
        if (bootCoreId == coreId) /* skip boot core */
        {
            NX_LOG_I("core#%d is boot core, skip it", coreId);
            continue;
        }
        NX_LOG_I("wakeup app core:%d", coreId);
        sbi_hsm_hart_start(coreId, MEM_KERNEL_BASE, 0);    
        NX_MemoryBarrier();
        NX_LOG_I("core#%d state:%d after wakeup", coreId, sbi_hsm_hart_status(coreId));
    }
    return NX_EOK;
}

NX_PRIVATE NX_Error HAL_CoreEnterApp(NX_UArch appCoreId)
{
    /* NOTE: init trap first before do anything */
    CPU_InitTrap(appCoreId);
    NX_LOG_I("core#%d enter application!", appCoreId);
    HAL_InitClock();
    PLIC_Init(NX_False);

    return NX_EOK;
    // return NX_EOK;
    /* don't sched now */
    // NX_IRQ_Enable();
    while (1)
    {
        NX_CASM("wfi");
    }    
    return NX_EOK;
}

NX_INTERFACE struct NX_SMP_Ops NX_SMP_OpsInterface = 
{
    .getIdx = HAL_CoreGetIndex,
    .bootApp = HAL_CoreBootApp,
    .enterApp = HAL_CoreEnterApp,
};
