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
    U32 backlink;
    U32 esp0;
    U32 ss0;
    U32 esp1;
    U32 ss1;
    U32 esp2;
    U32 ss2;
    U32 cr3;
    U32 eip;
    U32 eflags;
    U32 eax;
    U32 ecx;
    U32 edx;
    U32 ebx;
    U32 esp;
    U32 ebp;
    U32 esi;
    U32 edi;
    U32 es;
    U32 cs;
    U32 ss;
    U32 ds;
    U32 fs;
    U32 gs;
    U32 ldtr;
    U32 trap;
    U32 iobase;
};

void CPU_InitTSS(void);
struct CPU_TSS *CPU_GetTSS(void);
void CPU_SetTSS_Stack(Ubase top);

#endif  /* __I386_TSS__ */
