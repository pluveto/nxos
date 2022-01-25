/**
 * Copyright (c) 2018-2022, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Page init 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-28     JasonHu           Init
 */

#include <utils/memory.h>

#include <platform.h>
#include <page_zone.h>
#include <mm/page.h>
#include <mmu.h>
#include <riscv.h>

#define NX_LOG_LEVEL NX_LOG_INFO
#define NX_LOG_NAME "Page"
#include <utils/log.h>

#include <xbook/debug.h>
#include <drivers/direct_uart.h>

NX_PUBLIC MMU KernelMMU;

NX_PRIVATE NX_VOLATILE NX_U64 KernelTable[NX_PAGE_SIZE / sizeof(NX_U64)] NX_CALIGN(NX_PAGE_SIZE);

NX_PUBLIC void MMU_EarlyMap(void)
{
    MMU *mmu = &KernelMMU;
    MMU_InitTable(mmu, (void *)KernelTable, 0, MEM_KERNEL_TOP);

    /* map kernel self */
    MMU_MapPageWithPhy(mmu, MEM_KERNEL_BASE, MEM_KERNEL_BASE, KernelMMU.earlyEnd - MEM_KERNEL_BASE,
                       PAGE_DEFAULT_ATTR_KERNEL);
    /* uart0 */
    MMU_MapPageWithPhy(mmu, UART0_PHY_ADDR, UART0_PHY_ADDR, NX_PAGE_SIZE,
                       PAGE_DEFAULT_ATTR_KERNEL);
    /* CLINT */
    MMU_MapPageWithPhy(mmu, RISCV_CLINT_PADDR, RISCV_CLINT_PADDR, 0x10000,
                       PAGE_DEFAULT_ATTR_KERNEL);
    /* PLIC */
    MMU_MapPageWithPhy(mmu, RISCV_PLIC_PADDR, RISCV_PLIC_PADDR, 0x4000,
                       PAGE_DEFAULT_ATTR_KERNEL);
    MMU_MapPageWithPhy(mmu, RISCV_PLIC_PADDR + 0x200000, RISCV_PLIC_PADDR + 0x200000, 0x4000,
                       PAGE_DEFAULT_ATTR_KERNEL);
    
    NX_LOG_I("OS map early on [%p~%p]", MEM_KERNEL_BASE, KernelMMU.earlyEnd);

    MMU_SetPageTable((NX_Addr)mmu->table);
    
    NX_LOG_I("MMU enabled!");
}

NX_PUBLIC void *HAL_GetKernelPageTable(void)
{
    return KernelMMU.table;
}

/**
 * Init physic memory and map kernel on virtual memory.
 */
NX_PUBLIC void HAL_PageZoneInit(void)
{    
    NX_USize memSize = DRAM_SIZE_DEFAULT;
    
    NX_LOG_I("Memory NX_USize: %x Bytes %d MB", memSize, memSize / NX_MB);

    if (memSize == 0)
    {
        NX_PANIC("Get Memory NX_USize Failed!");
    }
    if (memSize < MEM_MIN_SIZE)
    {
        NX_LOG_E("Must has %d MB memory!", MEM_MIN_SIZE / NX_MB);
        NX_PANIC("Memory too small");
    }
    
    /* calc normal base & size */
    NX_USize normalSize = memSize - MEM_KERNEL_SZ;
    normalSize /= 2;
    if (normalSize > MEM_KERNEL_SPACE_SZ)
    {
        normalSize = MEM_KERNEL_SPACE_SZ;
    }
    
    /* calc user base & size */
    NX_Addr userBase = MEM_NORMAL_BASE + normalSize;
    NX_USize userSize = memSize - normalSize;

    NX_LOG_I("Normal memory: %p~%p NX_USize:%d MB", MEM_NORMAL_BASE, MEM_NORMAL_BASE + normalSize, normalSize / NX_MB);
    NX_LOG_I("User memory: %p~%p NX_USize:%d MB", userBase, userBase+ userSize, userSize / NX_MB);

    /* init page zone */
    NX_PageInitZone(NX_PAGE_ZONE_NORMAL, (void *)MEM_NORMAL_BASE, normalSize);
    NX_PageInitZone(NX_PAGE_ZONE_USER, (void *)userBase, userSize);

    KernelMMU.earlyEnd = MEM_NORMAL_BASE + normalSize;

    NX_LOG_I("Memroy init done.");
}

NX_IMPORT NX_Addr __NX_BssStart;
NX_IMPORT NX_Addr __NX_BssEnd;

NX_PUBLIC void HAL_ClearBSS(void)
{
    NX_MemZero(&__NX_BssStart, &__NX_BssEnd - &__NX_BssStart);
}
