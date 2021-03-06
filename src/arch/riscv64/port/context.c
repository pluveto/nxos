/**
 * Copyright (c) 2018-2022, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: NX_Thread context 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-16     JasonHu           Init
 */

#include <sched/context.h>
#include <utils/memory.h>
#include <utils/log.h>
#include <io/irq.h>
#include <context.h>
#include <regs.h>

NX_IMPORT void HAL_ContextSwitchNext(NX_Addr nextSP);
NX_IMPORT void HAL_ContextSwitchPrevNext(NX_Addr prevSP, NX_Addr nextSP);

NX_PRIVATE void *HAL_ContextInit(void *startEntry, void *exitEntry, void *arg, void *stackTop)
{
    NX_U8 *stack = NX_NULL;
    HAL_Context *context = NX_NULL;

    stack = (NX_U8 *)stackTop;
    stack = (NX_U8 *)NX_ALIGN_DOWN((NX_UArch)stack, sizeof(NX_UArch));

    stack -= sizeof(HAL_Context);
    context = (HAL_Context *)stack;
    
    NX_MemZero(context, sizeof(HAL_Context));

    context->a0 = (NX_UArch)arg;
    context->epc = (NX_UArch)startEntry;
    context->sp = (NX_UArch)(((NX_UArch)stack) + sizeof(HAL_Context));
    context->ra = (NX_UArch)exitEntry;
    
    /**
     * allow to access user space memory,
     * return to supervisor mode,
     * enable interrupt
     */
    context->sstatus = SSTATUS_SUM | SSTATUS_SPP | SSTATUS_SPIE;

    return (void *)stack;
}

NX_INTERFACE struct NX_ContextOps NX_ContextOpsInterface = 
{
    .init           = HAL_ContextInit,
    .switchNext     = HAL_ContextSwitchNext,
    .switchPrevNext = HAL_ContextSwitchPrevNext,
};
