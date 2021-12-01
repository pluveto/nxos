/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Riscv64 entry 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

    .section .text.start
    .extern OS_Main
    .global bootStack
    .global _Start
_Start:
    add t0, a0, 1
    slli t0, t0, 14
    la sp, bootStack
    add sp, sp, t0
	call OS_Main
loop:
    j loop

    .section .bss.stack
    .align 12
    .globl bootStack
bootStack:
    .space 4096 * 4 * 2
    .globl bootStack_top
bootStack_top:
