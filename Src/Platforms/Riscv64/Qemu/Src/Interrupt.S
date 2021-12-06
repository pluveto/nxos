/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Interrupt
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-03     JasonHu           Init
 */

#define __ASSEMBLY__
#include <Context.h>

.text

.extern IRQ_DelayQueueCheck
.extern TrapDispatch
.extern BootStackTop
.globl TrapEntry
.align 2 # TrapEntry must aligin with 4 byte
TrapEntry:

    /* save sp to sscratch reg, sscratch saved old sp from user/kernel */
    csrrw sp, sscratch, sp
    
    /* switch to cpu stack */
    la sp, BootStackTop

    /* save context to cpu stack */
    SAVE_CONTEXT
    
    /* call handler */
    mv a0, sp
    call TrapDispatch
    
    tail TrapReturn

.globl TrapReturn
TrapReturn:
    
    /* do exit check */
    call IRQ_DelayQueueCheck

    /* restore context will restore sp */
    RESTORE_CONTEXT
    /* return from supervisor call */
    sret
