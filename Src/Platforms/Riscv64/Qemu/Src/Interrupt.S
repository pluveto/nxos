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
.globl TrapEntry
.align 4 # TrapEntry must aligin with 4 byte
TrapEntry:
    SAVE_CONTEXT
    
    /* call handler */
    mv a0, sp
    call TrapDispatch
    
    tail TrapReturn

.globl TrapReturn
TrapReturn:
    
    /* do exit check */
    call IRQ_DelayQueueCheck

    RESTORE_CONTEXT
    # return from supervisor call
    sret
