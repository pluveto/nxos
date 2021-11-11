/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: I/O operate 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-9-23      JasonHu           Init
 */

#ifndef __PLATFORM_IO__
#define __PLATFORM_IO__

#include <XBook/Types.h>
#include <XBook/Defines.h>

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

#endif  /* __PLATFORM_IO__ */
