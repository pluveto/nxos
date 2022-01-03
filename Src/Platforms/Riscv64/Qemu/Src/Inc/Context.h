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

#include <XBook.h>
#include <Riscv.h>

#define CONTEXT_REG_NR  33

#define CTX_PC_OFF      0
#define CTX_SP_OFF      2
#define CTX_STATUS_OFF  32

#ifdef __ASSEMBLY__

/**
 * NOTE: 
 * sscratch was used to save sp for temp register when trap or context switch,
 * sp maybe user sp or kernel sp,
 * only save need sscratch, restore no need it.
 */
/* x1 broken in this func */
.macro SAVE_CONTEXT
    /* reserved context stack */
    addi sp, sp, -CONTEXT_REG_NR * REGBYTES

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
    
    csrr x1, sstatus
    STORE x1, CTX_STATUS_OFF * REGBYTES(sp)
    
    csrr x1, sepc
    STORE x1, CTX_PC_OFF * REGBYTES(sp)

    csrr x1, sscratch
    STORE x1, CTX_SP_OFF * REGBYTES(sp)

.endm

/* x1 broken in this func */
.macro RESTORE_CONTEXT

    LOAD x1, CTX_PC_OFF * REGBYTES(sp)
    csrw sepc, x1

    LOAD x1, CTX_STATUS_OFF * REGBYTES(sp)
    csrw sstatus, x1
    
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
#else

struct HAL_Context
{
    UArch epc;   //sepc
    UArch ra;    // Return address
    UArch sp;    // Stack pointer
    UArch gp;    // Global pointer
    UArch tp;    // Thread pointer
    UArch t0;    // Temporary
    UArch t1;    // Temporary
    UArch t2;    // Temporary
    UArch s0;    // Saved register/frame pointer
    UArch s1;    // Saved register
    UArch a0;    // Function argument/return value
    UArch a1;    // Function argument/return value
    UArch a2;    // Function argument
    UArch a3;    // Function argument
    UArch a4;    // Function argument
    UArch a5;    // Function argument
    UArch a6;    // Function argument
    UArch a7;    // Function argument
    UArch s2;    // Saved register
    UArch s3;    // Saved register
    UArch s4;    // Saved register
    UArch s5;    // Saved register
    UArch s6;    // Saved register
    UArch s7;    // Saved register
    UArch s8;    // Saved register
    UArch s9;    // Saved register
    UArch s10;   // Saved register
    UArch s11;   // Saved register
    UArch t3;    // Temporary
    UArch t4;    // Temporary
    UArch t5;    // Temporary
    UArch t6;    // Temporary
    UArch sstatus;// sstatus
} PACKED;
typedef struct HAL_Context HAL_Context;
#endif /* __ASSEMBLY__ */

#endif  /* __CONTEXT_HEADER__ */
