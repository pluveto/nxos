/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: platform context switch
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-16     JasonHu           Init
 */

#ifndef __PLATFORM_CONTEXT__
#define __PLATFORM_CONTEXT__

#include <XBook.h>

typedef void (*HAL_ThreadHandler)(void *);

struct HAL_Context
{
    U32 ebp;
    U32 ebx;
    U32 edi;
    U32 esi;

    /* first run point to handler, other time point to the ret addr of switch_to */
    void (*eip) (HAL_ThreadHandler handler, void *arg, void (*texit)());

    U32 unused;
    HAL_ThreadHandler handler;
    void *arg;
    void *exit; /* thread exit call */
};
typedef struct HAL_Context HAL_Context;

#endif  /* __PLATFORM_CONTEXT__ */
