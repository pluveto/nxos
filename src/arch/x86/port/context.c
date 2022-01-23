/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: NX_Thread context 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-16     JasonHu           Init
 */

#include <context.h>
#include <sched/context.h>
#include <interrupt.h>
#include <xbook/debug.h>
#include <io/irq.h>

NX_IMPORT void HAL_ContextSwitchNext(NX_Addr nextSP);
NX_IMPORT void HAL_ContextSwitchPrevNext(NX_Addr prevSP, NX_Addr nextSP);

/**
 * any thread will come here when first start
 */
NX_PRIVATE void ThreadEntry(HAL_ThreadHandler handler, void *arg, void (*texit)())
{
    NX_IRQ_Enable();
    handler(arg);
    if (texit)
        texit();
    NX_PANIC("Thread execute done, should never be here!");
}

NX_PRIVATE void *HAL_ContextInit(void *startEntry, void *exitEntry, void *arg, void *stackTop)
{
    NX_U8 *stack = NX_NULL;

    stack = stackTop + sizeof(NX_UArch);
    stack = (NX_U8 *)NX_ALIGN_DOWN((NX_UArch)stack, sizeof(NX_UArch));
    stack -= sizeof(HAL_TrapFrame);
    stack -= sizeof(HAL_Context);

    HAL_Context *context = (HAL_Context *)stack;
    context->eip = ThreadEntry;
    context->handler = startEntry;
    context->arg = arg;
    context->exit = exitEntry;
    context->ebp = context->ebx = context->esi = context->edi = 0;
    return stack;
}

NX_INTERFACE struct NX_ContextOps NX_ContextOpsInterface = 
{
    .init           = HAL_ContextInit,
    .switchNext     = HAL_ContextSwitchNext,
    .switchPrevNext = HAL_ContextSwitchPrevNext,
};
