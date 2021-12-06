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
#include <Utils/Memory.h>
#include <Utils/Log.h>
#include <Context.h>
#include <Regs.h>

INTERFACE U8 *HAL_ContextInit(void *entry, void *arg, U8 *stackTop, void *exit)
{
    U8 *stack = NULL;
    HAL_Context *context = NULL;

    stack = stackTop;
    stack = (U8 *)ALIGN_DOWN((Uint)stack, sizeof(Uint));

    stack -= sizeof(HAL_Context);
    context = (HAL_Context *)stack;
    
    Zero(context, sizeof(HAL_Context));

    context->a0 = (Uint)arg;
    context->epc = (Uint)entry;
    context->sp = (Uint)(((Uint)stack) + sizeof(HAL_Context));
    context->ra = (Uint)exit;
    
    /**
     * allow to access user space memory,
     * return to supervisor mode,
     * enable interrupt
     */
    context->sstatus = SSTATUS_SUM | SSTATUS_SPP | SSTATUS_SPIE;

    return stack;
}
