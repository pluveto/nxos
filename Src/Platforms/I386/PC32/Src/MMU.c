/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Memory Manage Unite 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-20     JasonHu           Init
 */

#include <MMU.h>
#include <Regs.h>

#define LOG_LEVEL LOG_INFO
#define LOG_NAME "MMU"
#include <Utils/Log.h>

PUBLIC void MMU_EarlyMap(MMU *mmu, Addr virStart, USize size)
{
    virStart = virStart & PAGE_ADDR_MASK;
    Addr phyStart = virStart;
    Addr virEnd = virStart + PAGE_ALIGNUP(size);

    LOG_I("OS map early on [%p~%p]", virStart, virEnd);
    
    MMU_PDE *pdt = (MMU_PDE *)mmu->table;
    
    USize pdeCnt = (virEnd - virStart) / (PTE_CNT_PER_PAGE * PAGE_SIZE);
    USize pteCnt = ((virEnd - virStart) / PAGE_SIZE) % PTE_CNT_PER_PAGE;
    Addr *pte = (UArch *) PAGE_TABLE_ADDR;
    U32 pdeIdx = PAGE_GET_L1(virStart);
    int i, j;
    for (i = 0; i < pdeCnt; i++)
    {
        /* fill pde */
        pdt[pdeIdx + i] = MAKE_PTE(pte, KERNEL_PAGE_ATTR);
        for (j = 0; j < PTE_CNT_PER_PAGE; j++)
        {
            /* fill each pte */
            pte[j] = MAKE_PTE(phyStart, KERNEL_PAGE_ATTR);
            phyStart += PAGE_SIZE;
        }
        pte += PTE_CNT_PER_PAGE;
    }
    if (pteCnt > 0)
    {
        /* fill left pte */
        pdt[pdeIdx + i] = MAKE_PTE(pte, KERNEL_PAGE_ATTR);
        for (j = 0; j < pteCnt; j++)
        {
            pte[j] = MAKE_PTE(phyStart, KERNEL_PAGE_ATTR);
            phyStart += PAGE_SIZE;
        }
    }
}

PUBLIC void MMU_SetPageTable(Addr addr)
{
    /* set new pgdir will flush tlb */
    CPU_WriteCR3(addr);
}

PUBLIC void MMU_Enable(void)
{
    CPU_WriteCR0(CPU_ReadCR0() | CR0_PG);
}
