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
#include <Page.h>

typedef Uint32 MMU_PDE; /* page dir entry */
typedef Uint32 MMU_PTE; /* page table entry */

struct MMU
{
    NOOPT U32 *table;
    Addr virStart;
    Addr earlyEnd;
    Addr virEnd;
};
typedef struct MMU MMU;

PUBLIC void MMU_EarlyMap(MMU *mmu, Addr virStart, Size size);
PUBLIC void MMU_SetPageTable(Addr addr);
PUBLIC void MMU_Enable(void);

#endif  /* __PLATFORM_MMU__ */
