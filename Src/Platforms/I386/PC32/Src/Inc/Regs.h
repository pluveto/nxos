/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: arch registers 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-22      JasonHu           Init
 */

#ifndef __PLATFROM_REGS__
#define __PLATFROM_REGS__

#include <Types.h>
#include <Defines.h>

#define EFLAGS_MBS    (1 << 1)
#define EFLAGS_IF_1   (1 << 9)
#define EFLAGS_IF_0 0
#define EFLAGS_IOPL_3 (3 << 12)
#define EFLAGS_IOPL_1 (1 << 12)
#define EFLAGS_IOPL_0 (0 << 12)

#define EFLAGS_IF (EFLAGS_IF_1)

/* cr0 bit 31 is page enable bit, 1: enable MMU, 0: disable MMU */
#define CR0_PG  (1 << 31)

/* cr0 bit 31 is page enable bit, 1: enable MMU, 0: disable MMU */
#define CR0_PG  (1 << 31)

INLINE void CPU_LoadTR(U32 selector)
{
    CASM("ltr %w0" : : "q" (selector));
}

INLINE void CPU_WriteCR3(U32 val)
{
    CASM("movl %0, %%cr3" : : "a" (val));
}

INLINE U32 CPU_ReadCR0()
{
    U32 val;
    CASM("movl %%cr0, %0\n\t": "=a" (val));
    return val;
}

INLINE void CPU_WriteCR0(U32 val)
{
    CASM("movl %0, %%cr0\n\t": :"a" (val));
}

#endif  /* __PLATFROM_REGS__ */
