/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Process for RISCV64
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2022-1-16      JasonHu           Init
 */

#include <Sched/Process.h>
#include <MM/Alloc.h>
#include <Utils/Memory.h>
#include <MM/Page.h>
#include <Utils/Log.h>
#include <XBook/Debug.h>
#include <Platform.h>
#include <MMU.h>

NX_PRIVATE NX_Error HAL_ProcessInitUserSpace(NX_Process *process)
{
    process->pageTable = NX_MemAlloc(NX_PAGE_SIZE);
    NX_ASSERT(process->pageTable != NX_NULL);
    NX_MemZero(process->pageTable, NX_PAGE_SIZE);
    NX_MemCopy(process->pageTable, (void *)HAL_GetKernelPageTable(), NX_PAGE_SIZE);
    return NX_EOK;
}

NX_PRIVATE NX_Error HAL_ProcessSwitchPageTable(void *pageTableVir)
{
    NX_Addr pageTablePhy = (NX_Addr)NX_Virt2Phy(pageTableVir);
    /* no need switch same page table */
    if (pageTablePhy != MMU_GetPageTable())
    {
        MMU_SetPageTable(pageTablePhy);
    }
    return NX_EOK;
}

NX_PRIVATE void *HAL_ProcessGetKernelPageTable(void)
{
    return HAL_GetKernelPageTable();
}

NX_INTERFACE struct NX_ProcessOps NX_ProcessOpsInterface = 
{
    .initUserSpace      = HAL_ProcessInitUserSpace,
    .switchPageTable    = HAL_ProcessSwitchPageTable,
    .getKernelPageTable = HAL_ProcessGetKernelPageTable,
};
