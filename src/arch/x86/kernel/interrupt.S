/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021/10/1      JasonHu      The first version
 */

.code32
.text

.extern HAL_InterruptDispatch
.extern NX_ReSchedCheck
.extern NX_IRQ_DelayQueueCheck

/* cpu no error code, need push by hand */
.macro CPU_TrapEntryNoErrCode p1
.global CPU_TrapEntry\p1
CPU_TrapEntry\p1:
    pushl $0x00
    
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    
    pushal

    movl %ss, %edx
    movl %edx, %ds
    movl %edx, %es

    pushl $\p1

    pushl %esp
    call HAL_InterruptDispatch
    addl $4, %esp

    jmp HAL_InterruptExit
.endm

/* cpu with error code */
.macro CPU_TrapEntry p1
.global CPU_TrapEntry\p1
CPU_TrapEntry\p1:
    nop
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushal

    movl %ss, %edx
    movl %edx, %ds
    movl %edx, %es

    pushl $\p1
    
    pushl %esp
    call HAL_InterruptDispatch
    addl $4, %esp

    jmp HAL_InterruptExit
.endm

CPU_TrapEntryNoErrCode 0x00
CPU_TrapEntryNoErrCode 0x01
CPU_TrapEntryNoErrCode 0x02
CPU_TrapEntryNoErrCode 0x03 
CPU_TrapEntryNoErrCode 0x04
CPU_TrapEntryNoErrCode 0x05
CPU_TrapEntryNoErrCode 0x06
CPU_TrapEntryNoErrCode 0x07 
CPU_TrapEntry 0x08
CPU_TrapEntryNoErrCode 0x09
CPU_TrapEntry 0x0a
CPU_TrapEntry 0x0b 
CPU_TrapEntryNoErrCode 0x0c
CPU_TrapEntry 0x0d
CPU_TrapEntry 0x0e
CPU_TrapEntryNoErrCode 0x0f 
CPU_TrapEntryNoErrCode 0x10
CPU_TrapEntry 0x11
CPU_TrapEntryNoErrCode 0x12
CPU_TrapEntryNoErrCode 0x13 
CPU_TrapEntryNoErrCode 0x14
CPU_TrapEntryNoErrCode 0x15
CPU_TrapEntryNoErrCode 0x16
CPU_TrapEntryNoErrCode 0x17 
CPU_TrapEntry 0x18
CPU_TrapEntryNoErrCode 0x19
CPU_TrapEntry 0x1a
CPU_TrapEntry 0x1b 
CPU_TrapEntryNoErrCode 0x1c
CPU_TrapEntry 0x1d
CPU_TrapEntry 0x1e
CPU_TrapEntryNoErrCode 0x1f 
CPU_TrapEntryNoErrCode 0x20
CPU_TrapEntryNoErrCode 0x21
CPU_TrapEntryNoErrCode 0x22
CPU_TrapEntryNoErrCode 0x23
CPU_TrapEntryNoErrCode 0x24
CPU_TrapEntryNoErrCode 0x25
CPU_TrapEntryNoErrCode 0x26
CPU_TrapEntryNoErrCode 0x27
CPU_TrapEntryNoErrCode 0x28
CPU_TrapEntryNoErrCode 0x29
CPU_TrapEntryNoErrCode 0x2a
CPU_TrapEntryNoErrCode 0x2b
CPU_TrapEntryNoErrCode 0x2c
CPU_TrapEntryNoErrCode 0x2d
CPU_TrapEntryNoErrCode 0x2e
CPU_TrapEntryNoErrCode 0x2f
CPU_TrapEntryNoErrCode 0x80

/* .extern SyscallDispath */

.global CPU_SyscallEntry
CPU_SyscallEntry:
    pushl $0x00
    
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    
    pushal

    movl %ss, %edx
    movl %edx, %ds
    movl %edx, %es

    pushl $0x80

    sti /* enable interrupt */

    /*
    pushl %esp
    call SyscallDispath
    addl $4, %esp
    */

    cli /* disable interrupt */
    
    jmp HAL_InterruptExit

.global HAL_SyscallExit
HAL_SyscallExit:
.global HAL_InterruptExit
HAL_InterruptExit:

    /* do exit check */
    call NX_IRQ_DelayQueueCheck
    
    /* check sched (Interrupt disabled) */
    call NX_ReSchedCheck

    /* do interrupt stack restore  */
    addl $4, %esp               /* skip intr no */

    popal
    
    popl %gs
    popl %fs
    popl %es
    popl %ds

    addl $4, %esp               /* skip error_code */

    iretl