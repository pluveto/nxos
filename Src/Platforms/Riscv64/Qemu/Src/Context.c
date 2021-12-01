/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Thread context 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-16     JasonHu           Init
 */

#include <XBook/HAL.h>

INTERFACE U8 *HAL_ContextInit(void *entry, void *arg, U8 *stackTop, void *exit)
{
    U8 *stack;
    stack = stackTop + sizeof(Uint);
    stack = (U8 *)ALIGN_DOWN((Uint)stack, sizeof(Uint));

    return stack;
}
