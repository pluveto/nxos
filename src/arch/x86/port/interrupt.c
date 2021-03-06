/**
 * Copyright (c) 2018-2022, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: interrupt
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

#include <gate.h>
#include <interrupt.h>
#include <utils/string.h>
#include <utils/memory.h>
#include <pic.h>
#include <io/irq.h>
#include <regs.h>

#define NX_LOG_LEVEL NX_LOG_DBG
#define NX_LOG_NAME "Interrupt"
#include <utils/log.h>

NX_PRIVATE char *ExceptionName[] = {
    "#DE Divide Error",
    "#DB Debug Exception",
    "NMI Interrupt",
    "#BP Breakpoint Exception",
    "#OF Overflow Exception",
    "#BR BOUND Range Exceeded Exception",
    "#UD Invalid Opcode Exception",
    "#NM Device Not Available Exception",
    "#DF Double Fault Exception",
    "Coprocessor Segment Overrun",
    "#TS Invalid TSS Exception",
    "#NP Segment Not Present",
    "#SS Stack Fault Exception",
    "#GP General Protection Exception",
    "#PF Page-Fault Exception",
    "Reserved",
    "#MF x87 FPU Floating-Point Error",
    "#AC Alignment Check Exception",
    "#MC Machine-Check Exception",
    "#XF SIMD Floating-Point Exception",
    "Unknown Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

NX_PUBLIC void CPU_InitInterrupt(void)
{
    PIC_Init();
}

NX_PUBLIC void CPU_TrapFrameDump(HAL_TrapFrame *frame)
{
    NX_LOG_E("edi:%x esi:%x ebp:%x esp dummy:%x ebx:%x edx:%x ecx:%x eax:%x",
            frame->edi, frame->esi, frame->ebp, frame->espDummy,
            frame->ebx, frame->edx, frame->ecx, frame->eax);
    NX_LOG_E("gs:%x fs:%x es:%x ds:%x error code:%x eip:%x cs:%x eflags:%x esp:%x ss:%x",
            frame->gs, frame->fs, frame->es, frame->ds, frame->errorCode,
            frame->eip, frame->cs, frame->eflags, frame->esp, frame->ss);
}

NX_PRIVATE void CPU_ExceptionDump(HAL_TrapFrame *frame)
{
    NX_LOG_E("Stack frame: exception name %s", ExceptionName[frame->vectorNumber]);
    if (frame->vectorNumber == 14)
    {
        NX_LOG_E("page fault addr: %x", CPU_ReadCR2());
    }
    CPU_TrapFrameDump(frame);
    if (frame->errorCode != 0xFFFFFFFF)
    {
        if (frame->errorCode & 1)
        {
            NX_LOG_E("    External Event: NMI,hard interruption,ect.");
        }
        else
        {
            NX_LOG_E("    Not External Event: inside.");
        }
        if (frame->errorCode & (1 << 1))
        {
            NX_LOG_E("    IDT: selector in idt.");
        }
        else
        {
            NX_LOG_E("    IDT: selector in gdt or ldt.");
        }
        if(frame->errorCode & (1 <<2 ))
        {
            NX_LOG_E("    TI: selector in ldt.");
        }
        else
        {
            NX_LOG_E("    TI: selector in gdt.");
        }
        NX_LOG_E("    Selector: idx %d", (frame->errorCode & 0xfff8) >> 3);
    }
}

NX_PUBLIC void HAL_InterruptDispatch(void *stackFrame)
{
    HAL_TrapFrame *frame = (HAL_TrapFrame *) stackFrame;
    NX_U32 vector = frame->vectorNumber;

    /* call handler with different vector */
    if (vector >= EXCEPTION_BASE && vector < EXCEPTION_BASE + MAX_EXCEPTION_NR)
    {
        /* exception */
        NX_LOG_E("unhandled exception vector %x/%s", vector, ExceptionName[vector]);
        CPU_ExceptionDump(frame);
        while (1);
    }
    else if (vector >= EXTERNAL_BASE && vector < EXTERNAL_BASE + NX_NR_IRQS)
    {
        NX_IRQ_Handle(vector - EXTERNAL_BASE);
        return;
    }
    else if (vector == SYSCALL_BASE)
    {
        /* syscall */
    }
    else
    {
        NX_LOG_W("uninstall intr vector %x", vector);
        while (1);
    }
}

NX_PRIVATE NX_Error HAL_IrqUnmask(NX_IRQ_Number irqno)
{
    if (irqno < 0 || irqno >= NX_NR_IRQS)
    {
        return NX_EINVAL;
    }

    PIC_Enable(irqno);
    return NX_EOK;
}

NX_PRIVATE NX_Error HAL_IrqMask(NX_IRQ_Number irqno)
{
    if (irqno < 0 || irqno >= NX_NR_IRQS)
    {
        return NX_EINVAL;
    }
    PIC_Disable(irqno);
    return NX_EOK;
}

NX_PRIVATE NX_Error HAL_IrqAck(NX_IRQ_Number irqno)
{
    if (irqno < 0 || irqno >= NX_NR_IRQS)
    {
        return NX_EINVAL;
    }
    PIC_Ack(irqno);
    return NX_EOK;
}

NX_PRIVATE void HAL_IrqEnable(void)
{
    NX_CASM("sti");
}

NX_PRIVATE void HAL_IrqDisable(void)
{
    NX_CASM("cli");
}

NX_PRIVATE NX_UArch HAL_IrqSaveLevel(void)
{
    NX_UArch level = 0;
    NX_CASM("pushfl; popl %0; cli":"=g" (level): :"memory");
    return level;
}

NX_PRIVATE void HAL_IrqRestoreLevel(NX_UArch level)
{
    NX_CASM("pushl %0; popfl": :"g" (level):"memory", "cc");
}

NX_INTERFACE NX_IRQ_Controller NX_IRQ_ControllerInterface = 
{
    .unmask = HAL_IrqUnmask,
    .mask = HAL_IrqMask,
    .ack = HAL_IrqAck,
    .enable = HAL_IrqEnable,
    .disable = HAL_IrqDisable,
    .saveLevel = HAL_IrqSaveLevel,
    .restoreLevel = HAL_IrqRestoreLevel,
};
