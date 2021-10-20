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

struct MMU
{
    Uint *table;
    Uint start;
    Uint end;
};
typedef struct MMU MMU;

#endif  /* __PLATFORM_MMU__ */
