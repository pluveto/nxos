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

#define NX_LOG_LEVEL NX_LOG_INFO
#define NX_LOG_NAME "MMU"
#include <Utils/Log.h>

NX_PUBLIC void MMU_EarlyMap(MMU *mmu, NX_Addr virStart, NX_USize size)
{
    virStart = virStart & NX_PAGE_ADDR_MASK;
    NX_Addr phyStart = virStart;
    NX_Addr virEnd = virStart + NX_PAGE_ALIGNUP(size);

    NX_LOG_I("OS map early on [%p~%p]", virStart, virEnd);
    
    MMU_PDE *pdt = (MMU_PDE *)mmu->table;
    
    NX_USize pdeCnt = (virEnd - virStart) / (PTE_CNT_PER_PAGE * NX_PAGE_SIZE);
    NX_USize pteCnt = ((virEnd - virStart) / NX_PAGE_SIZE) % PTE_CNT_PER_PAGE;
    NX_Addr *pte = (NX_UArch *) NX_PAGE_TABLE_ADDR;
    NX_U32 pdeIdx = NX_PAGE_GET_L1(virStart);
    int i, j;
    for (i = 0; i < pdeCnt; i++)
    {
        /* fill pde */
        pdt[pdeIdx + i] = MAKE_PTE(pte, KERNEL_PAGE_ATTR);
        for (j = 0; j < PTE_CNT_PER_PAGE; j++)
        {
            /* fill each pte */
            pte[j] = MAKE_PTE(phyStart, KERNEL_PAGE_ATTR);
            phyStart += NX_PAGE_SIZE;
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
            phyStart += NX_PAGE_SIZE;
        }
    }
}

NX_PUBLIC void MMU_SetPageTable(NX_Addr addr)
{
    /* set new pgdir will flush tlb */
    CPU_WriteCR3(addr);
}

NX_PUBLIC NX_Addr MMU_GetPageTable(void)
{
    return CPU_ReadCR3();
}

NX_PUBLIC void MMU_Enable(void)
{
    CPU_WriteCR0(CPU_ReadCR0() | CR0_PG);
}
