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

#ifndef __PLATFORM_MMU__
#define __PLATFORM_MMU__

#include <XBook.h>
#include <MM/Page.h>

#define PTE_SHIFT NX_PAGE_SHIFT
#define PTE_BITS 10
#define PDE_SHIFT (PTE_SHIFT + PTE_BITS)
#define PDE_BITS 10

#define NX_PAGE_PARTBIT(value, startBit, length) (((value) >> (startBit)) & ((1UL << (length)) - 1))

/* page offset */
#define NX_PAGE_GET_ID(addr) ((addr) >> NX_PAGE_SHIFT)
#define NX_PAGE_GET_OFFSET(addr) ((addr) & NX_PAGE_MASK)
#define NX_PAGE_GET_L1(addr) NX_PAGE_PARTBIT(addr, PDE_SHIFT, PDE_BITS)
#define NX_PAGE_GET_L2(addr) NX_PAGE_PARTBIT(addr, PTE_SHIFT, PTE_BITS)
#define NX_PAGE_GET_PADDR(pte) ((pte) & NX_PAGE_ADDR_MASK)
#define NX_PAGE_GET_PATTR(pte) ((pte) & NX_PAGE_MASK)

#define PTE_CNT_PER_PAGE 1024

#define NX_PAGE_TABLE_ADDR     0X3F3000

#define MAKE_PTE(paddr, attr) (NX_UArch) (((NX_UArch)(paddr) & NX_PAGE_ADDR_MASK) | ((attr) & NX_PAGE_MASK))

// page table entry (PTE) fields
#define PTE_P     0x001 // Present
#define PTE_R     0x000 // Read
#define PTE_W     0x002 // Write
#define PTE_X     0x000 // Execute
#define PTE_U     0x004 // User
#define PTE_PWT   0x008 // Write-through
#define PTE_S     0x000 // System
#define PTE_A     0x020 // Accessed
#define PTE_D     0x040 // Dirty

#define NX_PAGE_ATTR_RWX (PTE_X | PTE_W | PTE_R)
#define NX_PAGE_ATTR_RDONLY (PTE_R)
#define NX_PAGE_ATTR_RDEXE (PTE_X | PTE_R)

#define NX_PAGE_ATTR_USER (PTE_U)
#define NX_PAGE_ATTR_SYSTEM (PTE_S)

#define KERNEL_PAGE_ATTR  (PTE_P | NX_PAGE_ATTR_RWX | NX_PAGE_ATTR_SYSTEM)

#define PTE_VALID(pte) (*(pte) & PTE_P)

typedef NX_U32 MMU_PDE; /* page dir entry */
typedef NX_U32 MMU_PTE; /* page table entry */

struct MMU
{
    NX_VOLATILE NX_U32 *table;
    NX_Addr virStart;
    NX_Addr earlyEnd;
    NX_Addr virEnd;
};
typedef struct MMU MMU;

NX_PUBLIC void MMU_EarlyMap(MMU *mmu, NX_Addr virStart, NX_USize size);
NX_PUBLIC void MMU_SetPageTable(NX_Addr addr);
NX_PUBLIC NX_Addr MMU_GetPageTable(void);
NX_PUBLIC void MMU_Enable(void);

#endif  /* __PLATFORM_MMU__ */
