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

#ifndef __PLATFORM_MMU__
#define __PLATFORM_MMU__

#include <xbook.h>
#include <mm/page.h>

#define __PTE_SHIFT NX_PAGE_SHIFT
#define __PTE_BITS 10
#define __PDE_SHIFT (__PTE_SHIFT + __PTE_BITS)
#define __PDE_BITS 10
#define __PAGE_PARTBIT(value, startBit, length) (((value) >> (startBit)) & ((1UL << (length)) - 1))

/* page offset */
#define GET_PF_ID(addr) ((addr) >> NX_PAGE_SHIFT)
#define GET_PF_OFFSET(addr) ((addr) & NX_PAGE_MASK)
#define GET_PDE_OFF(addr) __PAGE_PARTBIT(addr, __PDE_SHIFT, __PDE_BITS)
#define GET_PTE_OFF(addr) __PAGE_PARTBIT(addr, __PTE_SHIFT, __PTE_BITS)
#define PTE2PADDR(pte) ((pte) & NX_PAGE_ADDR_MASK)
#define PTE2ATTR(pte) ((pte) & NX_PAGE_MASK)
#define PADDR2PTE(pa) (((NX_Addr)pa) & NX_PAGE_ADDR_MASK)

#define PTE_CNT_PER_PAGE 1024

/* map kernel early need some page tables */
#define EARLY_PAGE_TABLE_ADDR     0X3F3000

#define MAKE_PTE(paddr, attr) (NX_UArch) (((NX_UArch)(paddr) & NX_PAGE_ADDR_MASK) | ((attr) & NX_PAGE_MASK))

/* page table entry (PTE) fields */
#define PTE_P     0x001 // Present
#define PTE_R     0x000 // Read
#define PTE_W     0x002 // Write
#define PTE_X     0x000 // Execute
#define PTE_U     0x004 // User
#define PTE_PWT   0x008 // Write-through
#define PTE_S     0x000 // System
#define PTE_A     0x020 // Accessed
#define PTE_D     0x040 // Dirty

#define PAGE_ATTR_RWX (PTE_X | PTE_W | PTE_R)
#define PAGE_ATTR_RDONLY (PTE_R)
#define PAGE_ATTR_RDEXE (PTE_X | PTE_R)

#define PAGE_ATTR_USER (PTE_U)
#define PAGE_ATTR_SYSTEM (PTE_S)

#define KERNEL_PAGE_ATTR  (PTE_P | PAGE_ATTR_RWX | PAGE_ATTR_SYSTEM)

#define PTE_USED(pte) ((pte) & PTE_P)

typedef NX_U32 MMU_PDE; /* page dir entry */
typedef NX_U32 MMU_PTE; /* page table entry */

struct MMU
{
    MMU_PDE *table;
    NX_Addr virStart;
    NX_Addr earlyEnd;
    NX_Addr virEnd;
};
typedef struct MMU MMU;

NX_PUBLIC void MMU_EarlyMap(MMU *mmu, NX_Addr virStart, NX_USize size);
NX_PUBLIC void MMU_SetPageTable(NX_Addr addr);
NX_PUBLIC NX_Addr MMU_GetPageTable(void);
NX_PUBLIC void MMU_Enable(void);

NX_PUBLIC void MMU_InitTable(MMU *mmu, void *pageTable, NX_Addr virStart, NX_USize size);
NX_PUBLIC void *MMU_MapPage(MMU *mmu, NX_Addr virAddr, NX_USize size, NX_UArch attr);
NX_PUBLIC void *MMU_MapPageWithPhy(MMU *mmu, NX_Addr virAddr, NX_Addr phyAddr, NX_USize size, NX_UArch attr);
NX_PUBLIC NX_Error MMU_UnmapPage(MMU *mmu, NX_Addr virAddr, NX_USize size);
NX_PUBLIC void *MMU_Vir2Phy(MMU *mmu, NX_Addr virAddr);

#endif  /* __PLATFORM_MMU__ */
