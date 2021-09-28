/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: I386 registers 
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

INLINE u8 In8(u16 port)
{
    u8 data;
    CASM("inb %w1, %0" : "=a" (data) : "d" (port));
    return data;
}

INLINE u16 In16(u16 port)
{
    u16 data;
    CASM("inw %w1, %0" : "=a" (data) : "d" (port));
    return data;
}

INLINE u32 In32(u16 port)
{
    u32 data;
    CASM("inl %w1, %0" : "=a" (data) : "d" (port));
    return data;
}

INLINE void Out8(u16 port, u8 data)
{
    CASM("outb %0, %w1" : : "a" (data), "d" (port));
}

INLINE void Out16(u16 port, u16 data)
{
    CASM("outw %0, %w1" : : "a" (data), "d" (port));
}

INLINE void Out32(u16 port, u32 data)
{
    CASM("outl %0, %w1" : : "a" (data), "d" (port));
}

#endif  /* __I386_HEADER__ */
