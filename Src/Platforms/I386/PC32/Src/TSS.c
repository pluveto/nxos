/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: task status segment
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-9-17      JasonHu           Init
 */

#include <TSS.h>
#include <Segment.h>
#include <I386.h>
#include <Utils/Memory.h>

PRIVATE struct CPU_TSS tss;

PUBLIC struct CPU_TSS *CPU_GetTSS(void)
{
    return &tss;
}

PUBLIC void CPU_SetTSS_Stack(UBase top)
{
    tss.esp0 = top; /* esp0 is kernel mode */
}

void CPU_InitTSS(void)
{
    Zero(&tss, sizeof(struct CPU_TSS));
    tss.esp0 = KERNEL_STACK_TOP;
    tss.ss0 = KERNEL_DATA_SEL;
    tss.iobase = sizeof(struct CPU_TSS);

    CPU_LoadTR(KERNEL_TSS_SEL);
}
