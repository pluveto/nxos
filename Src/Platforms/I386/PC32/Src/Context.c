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

#include <Context.h>
#include <Sched/Context.h>
#include <Interrupt.h>
#include <XBook/Debug.h>
#include <IO/IRQ.h>

IMPORT void HAL_ContextSwitchNext(Addr nextSP);
IMPORT void HAL_ContextSwitchPrevNext(Addr prevSP, Addr nextSP);

/**
 * any thread will come here when first start
 */
PRIVATE void ThreadEntry(HAL_ThreadHandler handler, void *arg, void (*texit)())
{
    INTR_Enable();
    handler(arg);
    if (texit)
        texit();
    PANIC("Thread execute done, should never be here!");
}

PRIVATE void *HAL_ContextInit(void *startEntry, void *exitEntry, void *arg, void *stackTop)
{
    U8 *stack = NULL;

    stack = stackTop + sizeof(UArch);
    stack = (U8 *)ALIGN_DOWN((UArch)stack, sizeof(UArch));
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

INTERFACE struct ContextOps ContextOpsInterface = 
{
    .init           = HAL_ContextInit,
    .switchNext     = HAL_ContextSwitchNext,
    .switchPrevNext = HAL_ContextSwitchPrevNext,
};
