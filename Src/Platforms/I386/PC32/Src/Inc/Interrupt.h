/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Interrupt 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-3      JasonHu           Init
 */

#ifndef __PLATFORM_INTERRUPT__
#define __PLATFORM_INTERRUPT__

#define EXCEPTION_BASE 0x00
#define EXTERNAL_BASE 0x20
#define SYSCALL_BASE 0x80

#define MAX_EXCEPTION_NR 32

#define ERQ_PAGE_FAULT      14

/* irq number */
#define IRQ_CLOCK           (0)
#define IRQ_KEYBOARD        (1)
#define IRQ_CONNECT         (2)   /* connect to slave */
#define IRQ_SERIAL2         (3)
#define IRQ_SERIAL1         (4)
#define IRQ_PARALLEL2       (5)
#define IRQ_FLOPPY          (6)
#define IRQ_PARALLEL1       (7)

#define IRQ_RTCLOCK         (8)   /* real-time clock */
#define IRQ_REDIRECT        (9)   /* redirect to IRQ2 */
#define IRQ_RESERVED10      (10)
#define IRQ_RESERVED11      (11)
#define IRQ_MOUSE           (12)
#define IRQ_FPU             (13)
#define IRQ_HARDDISK        (14)
#define IRQ_RESERVE         (15)

struct HAL_TrapFrame
{
    U32 vectorNumber;

    U32 edi;
    U32 esi;
    U32 ebp;
    U32 espDummy; /* espdummy not used, only use a position */
    U32 ebx;
    U32 edx;
    U32 ecx;
    U32 eax;

    U32 gs;
    U32 fs;
    U32 es;
    U32 ds;
    U32 errorCode; /* error code will push into stack if exception has it, or not push 0 by ourself */
    U32 eip;
    U32 cs;
    U32 eflags;
    /* 
     * below will push into stack when from user privilege level enter
     * kernel privilege level (syscall/excption/interrupt) 
     */
    U32 esp;
    U32 ss;
} __attribute__((packed));
typedef struct HAL_TrapFrame HAL_TrapFrame;

PUBLIC void CPU_InitInterrupt(void);

#endif  /* __PLATFORM_INTERRUPT__ */
