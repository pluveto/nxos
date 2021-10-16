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

#endif  /* __XBOOK_DEFINES__ */
