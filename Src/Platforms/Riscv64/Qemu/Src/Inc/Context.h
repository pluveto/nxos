/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: context
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-2      JasonHu           Init
 */

#ifndef __CONTEXT_HEADER__
#define __CONTEXT_HEADER__

#include <Interrupt.h>

/**
 * NOTE: 
 * sscratch was used to save sp for temp register when trap or context switch,
 * sp maybe user sp or kernel sp,
 * only save need sscratch, restore no need it.
 */
#ifdef __ASSEMBLY__
/* t0, t1, t2 broken in this func */
.macro SAVE_CONTEXT
    /* reserved context stack */
    addi sp, sp, -CONTEXT_REG_NR * REGBYTES

    STORE x0, 0 * REGBYTES(sp)
    STORE x1, 1 * REGBYTES(sp)
    STORE x3, 3 * REGBYTES(sp)
    STORE x4, 4 * REGBYTES(sp)
    STORE x5, 5 * REGBYTES(sp)
    STORE x6, 6 * REGBYTES(sp)
    STORE x7, 7 * REGBYTES(sp)
    STORE x8, 8*REGBYTES(sp)
    STORE x9, 9*REGBYTES(sp)
    STORE x10, 10*REGBYTES(sp)
    STORE x11, 11*REGBYTES(sp)
    STORE x12, 12*REGBYTES(sp)
    STORE x13, 13*REGBYTES(sp)
    STORE x14, 14*REGBYTES(sp)
    STORE x15, 15*REGBYTES(sp)
    STORE x16, 16*REGBYTES(sp)
    STORE x17, 17*REGBYTES(sp)
    STORE x18, 18*REGBYTES(sp)
    STORE x19, 19*REGBYTES(sp)
    STORE x20, 20*REGBYTES(sp)
    STORE x21, 21*REGBYTES(sp)
    STORE x22, 22*REGBYTES(sp)
    STORE x23, 23*REGBYTES(sp)
    STORE x24, 24*REGBYTES(sp)
    STORE x25, 25*REGBYTES(sp)
    STORE x26, 26*REGBYTES(sp)
    STORE x27, 27*REGBYTES(sp)
    STORE x28, 28*REGBYTES(sp)
    STORE x29, 29*REGBYTES(sp)
    STORE x30, 30*REGBYTES(sp)
    STORE x31, 31*REGBYTES(sp)
    
    csrrw t0, sscratch, x0
    csrr t1, sstatus
    csrr t2, sepc

    STORE t0, CTX_SP_OFF * REGBYTES(sp)
    STORE t1, CTX_STATUS_OFF * REGBYTES(sp)
    STORE t2, CTX_PC_OFF * REGBYTES(sp)
.endm

/* s1, s2 broken in this func */
.macro RESTORE_CONTEXT
    LOAD s1, CTX_STATUS_OFF * REGBYTES(sp)
    LOAD s2, CTX_PC_OFF * REGBYTES(sp)

    csrw sstatus, s1
    csrw sepc, s2
    
    LOAD x1, 1*REGBYTES(sp)
    LOAD x3, 3*REGBYTES(sp)
    LOAD x4, 4*REGBYTES(sp)
    LOAD x5, 5*REGBYTES(sp)
    LOAD x6, 6*REGBYTES(sp)
    LOAD x7, 7*REGBYTES(sp)
    LOAD x8, 8*REGBYTES(sp)
    LOAD x9, 9*REGBYTES(sp)
    LOAD x10, 10*REGBYTES(sp)
    LOAD x11, 11*REGBYTES(sp)
    LOAD x12, 12*REGBYTES(sp)
    LOAD x13, 13*REGBYTES(sp)
    LOAD x14, 14*REGBYTES(sp)
    LOAD x15, 15*REGBYTES(sp)
    LOAD x16, 16*REGBYTES(sp)
    LOAD x17, 17*REGBYTES(sp)
    LOAD x18, 18*REGBYTES(sp)
    LOAD x19, 19*REGBYTES(sp)
    LOAD x20, 20*REGBYTES(sp)
    LOAD x21, 21*REGBYTES(sp)
    LOAD x22, 22*REGBYTES(sp)
    LOAD x23, 23*REGBYTES(sp)
    LOAD x24, 24*REGBYTES(sp)
    LOAD x25, 25*REGBYTES(sp)
    LOAD x26, 26*REGBYTES(sp)
    LOAD x27, 27*REGBYTES(sp)
    LOAD x28, 28*REGBYTES(sp)
    LOAD x29, 29*REGBYTES(sp)
    LOAD x30, 30*REGBYTES(sp)
    LOAD x31, 31*REGBYTES(sp)

    /* restore sp from context sp */
    LOAD sp, CTX_SP_OFF * REGBYTES(sp)
.endm

#endif

#endif  /* __CONTEXT_HEADER__ */
