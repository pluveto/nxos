/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: interrupt
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-2      JasonHu           Init
 */

#ifndef __INTERRUPT_HEADER__
#define __INTERRUPT_HEADER__

#include <XBook.h>

struct HAL_TrapFrame
{
    Uint zero;  // Hard-wired zero
    Uint ra;    // Return address
    Uint sp;    // Stack pointer
    Uint gp;    // Global pointer
    Uint tp;    // Thread pointer
    Uint t0;    // Temporary
    Uint t1;    // Temporary
    Uint t2;    // Temporary
    Uint s0;    // Saved register/frame pointer
    Uint s1;    // Saved register
    Uint a0;    // Function argument/return value
    Uint a1;    // Function argument/return value
    Uint a2;    // Function argument
    Uint a3;    // Function argument
    Uint a4;    // Function argument
    Uint a5;    // Function argument
    Uint a6;    // Function argument
    Uint a7;    // Function argument
    Uint s2;    // Saved register
    Uint s3;    // Saved register
    Uint s4;    // Saved register
    Uint s5;    // Saved register
    Uint s6;    // Saved register
    Uint s7;    // Saved register
    Uint s8;    // Saved register
    Uint s9;    // Saved register
    Uint s10;   // Saved register
    Uint s11;   // Saved register
    Uint t3;    // Temporary
    Uint t4;    // Temporary
    Uint t5;    // Temporary
    Uint t6;    // Temporary
    Uint status; //sstatus
    Uint epc; //sepc
    Uint tval; //trap value
    Uint cause; //scause
} PACKED;
typedef struct HAL_TrapFrame HAL_TrapFrame;

#endif  /* __INTERRUPT_HEADER__ */
