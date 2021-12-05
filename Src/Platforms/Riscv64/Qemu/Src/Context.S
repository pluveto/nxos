/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Thread context 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-01     JasonHu           Init
 */

.text

#define __ASSEMBLY__
#include <Context.h>
#include <Regs.h>

/**
 * void HAL_ContextSwitchNext(Addr nextSP);
 */
.globl HAL_ContextSwitchNext
HAL_ContextSwitchNext:
    LOAD sp, (a0)   /* sp = *nextSP */

    RESTORE_CONTEXT
    sret

/**
 * void HAL_ContextSwitchPrevNext(Addr prevSP, Addr nextSP);
 */
.globl HAL_ContextSwitchPrevNext
HAL_ContextSwitchPrevNext:
    /**
     * set as supervisor mode and enable interupt 
     * make sure kernel mode thread do context switch
     */
    li t1, SSTATUS_SPP | SSTATUS_SPIE 
    csrs sstatus, t1

    /* save kernel sp to sscratch */
    csrrw sp, sscratch, sp

    /**
     * set `sepc` as `ra`, `ra` saved return address of HAL_ContextSwitchPrevNext
     * when do `sret`, will return from HAL_ContextSwitchPrevNext
     */
    csrw sepc, ra
    
    /* save context to stack */
    SAVE_CONTEXT

    /* save sp to from thread, *from = `sp` */
    STORE sp, (a0)

    LOAD sp, (a1)   /* sp = *nextSP */

    /* restore context from stack */
    RESTORE_CONTEXT
    
    /* supervisor return, `pc` = `sepc` */
    sret
