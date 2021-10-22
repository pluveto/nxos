/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Configure for each platform
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-9-18      JasonHu           Init
 */

#ifndef __PLATFORM_PAGE__
#define __PLATFORM_PAGE__

#include <XBook.h>

#define PAGE_SHIFT  12
#define PAGE_SIZE   (1U << PAGE_SHIFT)
#define PAGE_MASK   (PAGE_SIZE - 1UL)

#define PAGE_UMASK  (~PAGE_MASK)
#define PAGE_ADDR_MASK PAGE_UMASK

#define PAGE_ALIGNUP(value) (((value) + PAGE_MASK) & PAGE_UMASK)
#define PAGE_ALIGNDOWN(value) ((value) & PAGE_UMASK)

#define PTE_SHIFT PAGE_SHIFT
#define PTE_BITS 10
#define PDE_SHIFT (PTE_SHIFT + PTE_BITS)
#define PDE_BITS 10

#define PAGE_PARTBIT(value, startBit, length) (((value) >> (startBit)) & ((1UL << (length)) - 1))

/* page offset */
#define PAGE_GET_ID(addr) ((addr) >> PAGE_SHIFT)
#define PAGE_GET_OFFSET(addr) ((addr) & PAGE_MASK)
#define PAGE_GET_L1(addr) PAGE_PARTBIT(addr, PDE_SHIFT, PDE_BITS)
#define PAGE_GET_L2(addr) PAGE_PARTBIT(addr, PTE_SHIFT, PTE_BITS)
#define PAGE_GET_PADDR(pte) ((pte) & PAGE_ADDR_MASK)
#define PAGE_GET_PATTR(pte) ((pte) & PAGE_MASK)

#define PTE_CNT_PER_PAGE 1024

#define PAGE_TABLE_ADDR     0X3F3000

#define MAKE_PTE(paddr, attr) (Uint) (((Uint)(paddr) & PAGE_ADDR_MASK) | ((attr) & PAGE_MASK))

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

#define PAGE_ATTR_RWX (PTE_X | PTE_W | PTE_R)
#define PAGE_ATTR_RDONLY (PTE_R)
#define PAGE_ATTR_RDEXE (PTE_X | PTE_R)

#define PAGE_ATTR_USER (PTE_U)
#define PAGE_ATTR_SYSTEM (PTE_S)

#define KERNEL_PAGE_ATTR  (PTE_P | PAGE_ATTR_RWX | PAGE_ATTR_SYSTEM)

#define PTE_VALID(pte) (*(pte) & PTE_P)

/**
 * Must define & init PageZone in each paltfrom 
 */
enum PageZone
{
    PZ_NORMAL = 0,
    PZ_DMA,
    PZ_USER,
    PZ_NR,  /* Must be last member */
};
typedef enum PageZone PageZone;

#endif  /* __PLATFORM_PAGE__ */
