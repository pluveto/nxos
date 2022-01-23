/**
 * Copyright (c) 2018-2022, BookOS Development Team
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

#include <xbook.h>

typedef void (*HAL_ThreadHandler)(void *);

struct HAL_Context
{
    NX_U32 ebp;
    NX_U32 ebx;
    NX_U32 edi;
    NX_U32 esi;

    /* first run point to handler, other time point to the ret addr of switch_to */
    void (*eip) (HAL_ThreadHandler handler, void *arg, void (*texit)());

    NX_U32 unused;
    HAL_ThreadHandler handler;
    void *arg;
    void *exit; /* thread exit call */
};
typedef struct HAL_Context HAL_Context;

#endif  /* __PLATFORM_CONTEXT__ */
