/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Memory Manage Unite 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2022-1-16      JasonHu           Init
 */

#ifndef __PLATFORM_MMU__
#define __PLATFORM_MMU__

#include <xbook.h>
#include <mm/page.h>

// page table entry (PTE) fields
#define PTE_V     0x001 // Valid
#define PTE_R     0x002 // Read
#define PTE_W     0x004 // Write
#define PTE_X     0x008 // Execute
#define PTE_U     0x010 // User
#define PTE_G     0x020 // Global
#define PTE_A     0x040 // Accessed
#define PTE_D     0x080 // Dirty
#define PTE_SOFT  0x300 // Reserved for Software

#define PTE_PPN_SHIFT 10

#define PAGE_ATTR_NEXT_LEVEL (0)
#define PAGE_ATTR_RWX (PTE_X | PTE_W | PTE_R)
#define PAGE_ATTR_READONLY (PTE_R)
#define PAGE_ATTR_READEXECUTE (PTE_X | PTE_R)

#define PAGE_ATTR_USER (PTE_U)
#define PAGE_ATTR_SYSTEM (0)

#define PAGE_DEFAULT_ATTR_LEAF (PAGE_ATTR_RWX | PAGE_ATTR_USER | PTE_V | PTE_G)
#define PAGE_DEFAULT_ATTR_NEXT (PAGE_ATTR_NEXT_LEVEL | PTE_V | PTE_G)
#define PAGE_DEFAULT_ATTR_KERNEL (PAGE_ATTR_RWX | PAGE_ATTR_SYSTEM | PTE_V | PTE_G)

#define PAGE_IS_LEAF(pte) ((pte) & PAGE_ATTR_RWX)
#define PTE_USED(pte) ((pte) & PTE_V)

#define GET_PF_ID(addr) ((addr) >> NX_PAGE_SHIFT)
#define GET_PF_OFFSET(addr) ((addr) & NX_PAGE_MASK)

/* extract the three 9-bit page table indices from a virtual address. */
#define VPN_MASK          0x1FF // 9 bits
#define VPN_SHIFT(level)  (NX_PAGE_SHIFT + (9 * (level)))
#define GET_LEVEL_OFF(level, va) ((((NX_Addr)(va)) >> VPN_SHIFT(level)) & VPN_MASK)

/* bit 0-9 is attr, bit 10 ~ 37 is PPN */
#define PTE2PADDR(pte) ((((pte) >> 10) & 0xFFFFFFF) << NX_PAGE_SHIFT)
#define PADDR2PTE(pa) ((((NX_Addr)pa) >> 12) << 10)

/* use sv39 mmu mode */
#define MMU_MODE_SV39   0x08
#define MMU_MODE_BIT_SHIFT   60
#define MAKE_SATP(pageTable) (((NX_USize)MMU_MODE_SV39 << MMU_MODE_BIT_SHIFT) | (((NX_Addr)pageTable) >> NX_PAGE_SHIFT))

#define GET_ADDR_FROM_SATP(satp) ((((NX_Addr)satp) << NX_PAGE_SHIFT))

NX_INLINE void SFenceVMA()
{
    NX_CASM("sfence.vma");
}

#define MMU_FlushTLB() SFenceVMA()

typedef NX_U64 MMU_PDE; /* page dir entry */
typedef NX_U64 MMU_PTE; /* page table entry */

struct MMU
{
    MMU_PDE *table;
    NX_Addr virStart;
    NX_Addr virEnd;
    NX_Addr earlyEnd;
};
typedef struct MMU MMU;

NX_PUBLIC void MMU_InitTable(MMU *mmu, void *pageTable, NX_Addr virStart, NX_USize size);
NX_PUBLIC void MMU_SetPageTable(NX_Addr addr);
NX_PUBLIC NX_Addr MMU_GetPageTable(void);

NX_PUBLIC void *MMU_MapPage(MMU *mmu, NX_Addr virAddr, NX_USize size, NX_UArch attr);
NX_PUBLIC void *MMU_MapPageWithPhy(MMU *mmu, NX_Addr virAddr, NX_Addr phyAddr, NX_USize size, NX_UArch attr);
NX_PUBLIC NX_Error MMU_UnmapPage(MMU *mmu, NX_Addr virAddr, NX_USize size);
NX_PUBLIC void *MMU_Vir2Phy(MMU *mmu, NX_Addr virAddr);

#endif  /* __PLATFORM_MMU__ */
