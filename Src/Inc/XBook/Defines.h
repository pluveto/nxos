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

#ifndef __XBOOK_DEFINES__
#define __XBOOK_DEFINES__

#define NULL ((void *) 0)

/**
 * Private variable or function 
 */
#define PRIVATE static

/**
 * Public variable or function 
 */
#define PUBLIC 

/**
 * Operating system interface
 */
#define INTERFACE PUBLIC 

/**
 * Import variable or function 
 */
#define IMPORT extern

/**
 * Export variable or function
 * TODO: add to link module 
 */
#define EXPORT

/**
 * Inline function 
 * 
 */
#define INLINE  static inline

/**
 * Inline assembly 
 */
#define CASM    __asm__ __volatile__

/**
 * Align
 */
#define ALIGN_UP(x, y) ((x + (y - 1)) & (~(y - 1)))
#define ALIGN_DOWN(x, y) ((x) & ~((y) - 1))

/**
 * Offset of struct
 */
#define OffsetOfStruct(type, member) ((Size) &((type *)0)->member)

/**
 * Ptr of struct base addr 
 */
#define PtrOfStruct(ptr, type, member) ({ \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - OffsetOfStruct(type,member) ); \
})

#define TypeCast(t, exp) ((t)(exp))
#define ArraryCast(p, size) (((char(*)[size])(p)))

#define MIN(x, y) ({typeof(x) _x = (x);typeof(y) _y = (y);(void)(&_x == &_y);_x < _y ? _x : _y; })

#define SZ_KB   1024
#define SZ_MB   (SZ_KB * 1024)
#define SZ_GB   (SZ_MB * 1024)
#define SZ_TB   (SZ_GB * 1024)

/**
 * Compile Align
 */
#define CALIGN(size) __attribute__((aligned(size)))

/**
 * Make sure no compile optimization
 */
#define NOOPT volatile 

#define DIV_ROUND_UP(x, step) (((x) + (step) - 1) / (step))
#define DIV_ROUND_DOWN(x, step) ((x) / (step))

/**
 * calc array element count
 */
#define ARRAY_SIZE(a) (sizeof (a) / sizeof ((a)[0]))

/**
 * weak symble
 */
#define WEAK_SYM __attribute__((weak))

#define SECTION(x)          __attribute__((section(x)))
#define UNUSED              __attribute__((unused))
#define USED                __attribute__((used))
#define ALIGN(n)            __attribute__((aligned(n)))

#endif  /* __XBOOK_DEFINES__ */
