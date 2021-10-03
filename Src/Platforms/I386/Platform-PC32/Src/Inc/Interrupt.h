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

#define MAX_INTR_NR 0x81
#define ERQ_BASE 0x00
#define IRQ_BASE 0x20
#define SYSCALL_BASE 0x80

#define MAX_IRQ_NR 16

#define ERQ_PAGE_FAULT      14

#define IRQ_CLOCK           0
#define IRQ_KEYBOARD        1
#define IRQ_CONNECT         2   /* connect to slave */
#define IRQ_SERIAL2         3
#define IRQ_SERIAL1         4
#define IRQ_PARALLEL2       5
#define IRQ_FLOPPY          6
#define IRQ_PARALLEL1       7

#define IRQ_RTCLOCK         8   /* real-time clock */
#define IRQ_REDIRECT        9   /* redirect to IRQ2 */
#define IRQ_RESERVED10      10
#define IRQ_RESERVED11      11
#define IRQ_MOUSE           12
#define IRQ_FPU             13
#define IRQ_HARDDISK        14
#define IRQ_RESERVE         15

#endif  /* __PLATFORM_INTERRUPT__ */
