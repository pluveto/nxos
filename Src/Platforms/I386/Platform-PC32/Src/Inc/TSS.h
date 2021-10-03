/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: x86 tss
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-3      JasonHu           Init
 */

#ifndef __I386_TSS__
#define __I386_TSS__

#include <XBook.h>

#define KERNEL_STACK_TOP_PHY 0x9f000
#define KERNEL_STACK_TOP (KERNEL_VADDR_START + KERNEL_STACK_TOP_PHY)

struct CPU_TSS
{
    u32 backlink;
    u32 esp0;
    u32 ss0;
    u32 esp1;
    u32 ss1;
    u32 esp2;
    u32 ss2;
    u32 cr3;
    u32 eip;
    u32 eflags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u32 es;
    u32 cs;
    u32 ss;
    u32 ds;
    u32 fs;
    u32 gs;
    u32 ldtr;
    u32 trap;
    u32 iobase;
};

void CPU_InitTSS(void);
struct CPU_TSS *CPU_GetTSS(void);
void CPU_SetTSS_Stack(ubase top);

#endif  /* __I386_TSS__ */
