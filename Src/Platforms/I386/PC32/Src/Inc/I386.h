/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: I386 arch special 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-9-23      JasonHu           Init
 */

#ifndef __I386_HEADER__
#define __I386_HEADER__

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

INLINE U8 IO_In8(U16 port)
{
    U8 data;
    CASM("inb %w1, %0" : "=a" (data) : "d" (port));
    return data;
}

INLINE U16 IO_In16(U16 port)
{
    U16 data;
    CASM("inw %w1, %0" : "=a" (data) : "d" (port));
    return data;
}

INLINE U32 IO_In32(U16 port)
{
    U32 data;
    CASM("inl %w1, %0" : "=a" (data) : "d" (port));
    return data;
}

INLINE void IO_Out8(U16 port, U8 data)
{
    CASM("outb %0, %w1" : : "a" (data), "d" (port));
}

INLINE void IO_Out16(U16 port, U16 data)
{
    CASM("outw %0, %w1" : : "a" (data), "d" (port));
}

INLINE void IO_Out32(U16 port, U32 data)
{
    CASM("outl %0, %w1" : : "a" (data), "d" (port));
}

INLINE void CPU_LoadTR(U32 selector)
{
    CASM("ltr %w0" : : "q" (selector));
}

#endif  /* __I386_HEADER__ */
