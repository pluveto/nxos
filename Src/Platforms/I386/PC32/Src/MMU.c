/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Memory Manage Unite 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-20     JasonHu           Init
 * 2022-1-20      JasonHu           add map & unmap
 */

#include <MMU.h>
#include <Regs.h>
#include <XBook/Debug.h>
#include <IO/IRQ.h>
#include <Utils/Memory.h>

#define NX_LOG_LEVEL NX_LOG_INFO
#define NX_LOG_NAME "MMU"
#include <Utils/Log.h>

NX_PRIVATE NX_Error UnmapOnePage(MMU *mmu, NX_Addr virAddr);
NX_INLINE NX_Error __UnmapPage(MMU *mmu, NX_Addr virAddr, NX_USize pages);

NX_PRIVATE MMU_PTE *PageWalk(MMU_PDE *pageTable, NX_Addr virAddr, NX_Bool allocPage)
{
    MMU_PTE *pte = &pageTable[GET_PDE_OFF(virAddr)];
    
    if (PTE_USED(*pte))
    {
        pageTable = (MMU_PDE *)PTE2PADDR(*pte);
    }
    else
    {
        if (allocPage == NX_False)
        {
            return NX_NULL;
        }
        pageTable = (MMU_PDE *)NX_PageAlloc(1);
        if (pageTable == NX_NULL)
        {
            return NX_NULL;
        }
        NX_MemZero(NX_Phy2Virt(pageTable), NX_PAGE_SIZE);

        /* increase page table reference */
        void *levelPageTable = (void *)(NX_Virt2Phy((NX_Addr)pte) & NX_PAGE_ADDR_MASK);
        NX_PageIncrease(levelPageTable);
        
        *pte = PADDR2PTE(pageTable) | PTE_P;
    }
    pageTable = (MMU_PDE *)NX_Phy2Virt(pageTable);

    return &pageTable[GET_PTE_OFF(virAddr)];
}

/**
 * walk addr for get pte level 0, 1
 */
NX_PRIVATE NX_Error PageWalkPTE(MMU_PDE *pageTable, NX_Addr virAddr, MMU_PTE *pteArray[2])
{
    MMU_PTE *pte = (MMU_PTE *)&pageTable[GET_PDE_OFF(virAddr)];
    pteArray[1] = pte;

    if (PTE_USED(*pte))
    {
        pageTable = (MMU_PTE *)PTE2PADDR(*pte);        
        pageTable = (MMU_PTE *)NX_Phy2Virt(pageTable);
    }
    else
    {
        NX_LOG_E("map walk pte: pte on vir:%p not used!", virAddr);
        return NX_EFAULT;
    }
    pteArray[0] = &pageTable[GET_PTE_OFF(virAddr)];
    return NX_EOK;
}

NX_PRIVATE NX_Bool IsVirAddrMapped(MMU *mmu, NX_Addr virAddr)
{
    MMU_PTE *pte = PageWalk(mmu->table, virAddr, NX_False);
    if (pte == NX_NULL)
    {
        return NX_False;
    }
    if (!PTE_USED(*pte))
    {
        return NX_False;
    }
    return NX_True;
}

NX_PRIVATE NX_Error MapOnePage(MMU *mmu, NX_Addr virAddr, NX_Addr phyAddr, NX_UArch attr)
{
    if (IsVirAddrMapped(mmu, virAddr) == NX_True)
    {
        NX_LOG_E("map page: vir:%p was mapped!", virAddr);
        return NX_EINVAL;
    }

    MMU_PDE *pageTable = (MMU_PDE *)mmu->table;

    MMU_PTE *pte = PageWalk(pageTable, virAddr, NX_True);
    if (pte == NX_NULL)
    {
        NX_LOG_E("map page: walk page vir:%p failed!", virAddr);
        return NX_EFAULT;
    }
    if (PTE_USED(*pte))
    {
        NX_PANIC("Map one page but PTE had used!");
    }
    /* increase last level page table reference */
    void *levelPageTable = (void *)(NX_Virt2Phy((NX_Addr)pte) & NX_PAGE_ADDR_MASK);
    NX_PageIncrease(levelPageTable);
    
    *pte = PADDR2PTE(phyAddr) | attr | PTE_P;

    return NX_EOK;
}

NX_PRIVATE void *__MapPageWithPhy(MMU *mmu, NX_Addr virAddr, NX_Addr phyAddr, NX_USize size, NX_UArch attr)
{
    NX_Addr addrStart = phyAddr;
    NX_Addr addrEnd = phyAddr + size - 1;

    NX_ISize pages = GET_PF_ID(addrEnd) - GET_PF_ID(addrStart) + 1;
    NX_USize mappedPages = 0;

    while (pages > 0)
    {
        if (MapOnePage(mmu, virAddr, phyAddr, attr) != NX_EOK)
        {
            NX_LOG_E("map page: vir:%p phy:%p attr:%x failed!", virAddr, phyAddr, attr);
            __UnmapPage(mmu, addrStart, mappedPages);
            return NX_NULL;
        }
        virAddr += NX_PAGE_SIZE;
        phyAddr += NX_PAGE_SIZE;
        pages--;
        mappedPages++;
    }
    return (void *)addrStart;
}

NX_PRIVATE void *__MapPage(MMU *mmu, NX_Addr virAddr, NX_USize size, NX_UArch attr)
{
    NX_Addr addrStart = virAddr;
    NX_Addr addrEnd = virAddr + size - 1;

    NX_ISize pages = GET_PF_ID(addrEnd) - GET_PF_ID(addrStart) + 1;
    NX_USize mappedPages = 0;
    void *phyAddr;

    while (pages > 0)
    {
        phyAddr = NX_PageAlloc(1);
        if (phyAddr == NX_NULL)
        {
            NX_LOG_E("map page: alloc page failed!");
            goto err;
        }

        if (MapOnePage(mmu, virAddr, (NX_Addr)phyAddr, attr) != NX_EOK)
        {
            NX_LOG_E("map page: vir:%p phy:%p attr:%x failed!", virAddr, phyAddr, attr);
            goto err;
        }
        virAddr += NX_PAGE_SIZE;
        phyAddr += NX_PAGE_SIZE;
        pages--;
        mappedPages++;
    }
    return (void *)addrStart;
err:
    __UnmapPage(mmu, addrStart, mappedPages);
    return NX_NULL;
}

NX_PUBLIC void *MMU_MapPage(MMU *mmu, NX_Addr virAddr, NX_USize size, NX_UArch attr)
{
    virAddr = virAddr & NX_PAGE_ADDR_MASK;
    size = NX_PAGE_ALIGNUP(size);
    
    NX_UArch level = NX_IRQ_SaveLevel();
    void *addr = __MapPage(mmu, virAddr, size, attr);
    NX_IRQ_RestoreLevel(level);
    return addr;
}

NX_PUBLIC void *MMU_MapPageWithPhy(MMU *mmu, NX_Addr virAddr, NX_Addr phyAddr, NX_USize size, NX_UArch attr)
{
    virAddr = virAddr & NX_PAGE_ADDR_MASK;
    phyAddr = phyAddr & NX_PAGE_ADDR_MASK;
    size = NX_PAGE_ALIGNUP(size);
    
    NX_UArch level = NX_IRQ_SaveLevel();
    void *addr = __MapPageWithPhy(mmu, virAddr, phyAddr, size, attr);
    NX_IRQ_RestoreLevel(level);
    return addr;
}

NX_PRIVATE NX_Error UnmapOnePage(MMU *mmu, NX_Addr virAddr)
{
    MMU_PDE *pde, *pageTable = (MMU_PDE *)mmu->table;
    MMU_PTE *pte;
    NX_Addr phyPage;
    void *levelPageTable;

    MMU_PTE *pteArray[2] = {NX_NULL, NX_NULL};
    NX_ASSERT(PageWalkPTE(pageTable, virAddr, pteArray) == NX_EOK);

    pte = pteArray[0];
    NX_ASSERT(pte != NX_NULL);
    NX_ASSERT(PTE_USED(*pte));
    phyPage = PTE2PADDR(*pte);
    NX_ASSERT(phyPage);
    NX_PageFree((void *)phyPage);   /* free leaf page*/
    *pte = 0; /* clear pte */
    
    /* free none-leaf page */
    levelPageTable = (void *)(((NX_Addr)pte) & NX_PAGE_ADDR_MASK); /* get level page table by pte */
    if (NX_PageFree(levelPageTable) == NX_EOK)
    {
        pde = pteArray[1];
        NX_ASSERT(PTE_USED(*pde));
        NX_ASSERT((NX_Addr)levelPageTable == PTE2PADDR(*pde));
        *pde = 0;   /* clear pde */
    }
    return NX_EOK;
}

NX_INLINE NX_Error __UnmapPage(MMU *mmu, NX_Addr virAddr, NX_USize pages)
{
    while (pages > 0)
    {
        UnmapOnePage(mmu, virAddr);
        virAddr += NX_PAGE_SIZE;
        pages--;
    }
    return NX_EOK;
}

NX_PUBLIC NX_Error MMU_UnmapPage(MMU *mmu, NX_Addr virAddr, NX_USize size)
{
    virAddr = virAddr & NX_PAGE_ADDR_MASK;
    size = NX_PAGE_ALIGNUP(size);
    
    NX_Addr addrStart = virAddr;
    NX_Addr addrEnd = virAddr + size - 1;
    NX_USize pages = GET_PF_ID(addrEnd) - GET_PF_ID(addrStart) + 1;

    NX_UArch level = NX_IRQ_SaveLevel();
    NX_Error err = __UnmapPage(mmu, virAddr, pages);
    NX_IRQ_RestoreLevel(level);
    return err;
}

NX_PUBLIC void *MMU_Vir2Phy(MMU *mmu, NX_Addr virAddr)
{
    NX_Addr pagePhy;
    NX_Addr pageOffset;
    
    MMU_PDE *pageTable = (MMU_PDE *)mmu->table;

    MMU_PTE *pte = PageWalk(pageTable, virAddr, NX_False);
    if (pte == NX_NULL)
    {
        NX_PANIC("vir2phy walk fault!");
    }

    if (!PTE_USED(*pte))
    {
        NX_PANIC("vir2phy pte not used!");
    }

    pagePhy = PTE2PADDR(*pte);
    pageOffset = virAddr % NX_PAGE_SIZE;
    return (void *)(pagePhy + pageOffset);
}

NX_PUBLIC void MMU_InitTable(MMU *mmu, void *pageTable, NX_Addr virStart, NX_USize size)
{
    mmu->table = pageTable;
    mmu->virStart = virStart & NX_PAGE_ADDR_MASK;
    mmu->virEnd = virStart + NX_PAGE_ALIGNUP(size);
}

NX_PUBLIC void MMU_EarlyMap(MMU *mmu, NX_Addr virStart, NX_USize size)
{
    virStart = virStart & NX_PAGE_ADDR_MASK;
    NX_Addr phyStart = virStart;
    NX_Addr virEnd = virStart + NX_PAGE_ALIGNUP(size);

    NX_LOG_I("OS map early on [%p~%p]", virStart, virEnd);
    
    MMU_PDE *pdt = (MMU_PDE *)mmu->table;
    
    NX_USize pdeCnt = (virEnd - virStart) / (PTE_CNT_PER_PAGE * NX_PAGE_SIZE);
    NX_USize pteCnt = ((virEnd - virStart) / NX_PAGE_SIZE) % PTE_CNT_PER_PAGE;
    NX_Addr *pte = (NX_UArch *) EARLY_PAGE_TABLE_ADDR;
    NX_U32 pdeIdx = GET_PDE_OFF(virStart);
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
