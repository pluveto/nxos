/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: interrupt
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

#include <Gate.h>
#include <Interrupt.h>
#include <Utils/String.h>
#include <Utils/Memory.h>
#include <PIC.h>
#include <IO/IRQ.h>

#define LOG_LEVEL LOG_DBG
#define LOG_NAME "Interrupt"
#include <Utils/Log.h>

PRIVATE char *ExceptionName[] = {
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

PUBLIC void CPU_InitInterrupt(void)
{
    PIC_Init();
}

PUBLIC void InterruptDispatch(void *stackFrame)
{
    HAL_TrapFrame *frame = (HAL_TrapFrame *) stackFrame;
    U32 vector = frame->vectorNumber;

    /* call handler with different vector */
    if (vector >= EXCEPTION_BASE && vector < EXCEPTION_BASE + MAX_EXCEPTION_NR)
    {
        /* exception */
        LOG_E("unhandled exception vector %x/%s", vector, ExceptionName[vector]);
        while (1);
    }
    else if (vector >= EXTERNAL_BASE && vector < EXTERNAL_BASE + NR_IRQS)
    {
        IRQ_Handle(vector - EXTERNAL_BASE);
        return;
    }
    else if (vector == SYSCALL_BASE)
    {
        /* syscall */
    }
    else
    {
        LOG_W("uninstall intr vector %x", vector);
        while (1);
    }
}

PRIVATE OS_Error HAL_IrqUnmask(IRQ_Number irqno)
{
    if (irqno < 0 || irqno >= NR_IRQS)
    {
        return OS_EINVAL;
    }

    PIC_Enable(irqno);
    return OS_EOK;
}

PRIVATE OS_Error HAL_IrqMask(IRQ_Number irqno)
{
    if (irqno < 0 || irqno >= NR_IRQS)
    {
        return OS_EINVAL;
    }
    PIC_Disable(irqno);
    return OS_EOK;
}

PRIVATE OS_Error HAL_IrqAck(IRQ_Number irqno)
{
    if (irqno < 0 || irqno >= NR_IRQS)
    {
        return OS_EINVAL;
    }
    PIC_Ack(irqno);
    return OS_EOK;
}

PRIVATE void HAL_IrqEnable(void)
{
    CASM("sti");
}

PRIVATE void HAL_IrqDisable(void)
{
    CASM("cli");
}

PRIVATE UArch HAL_IrqSaveLevel(void)
{
    UArch level = 0;
    CASM("pushfl; popl %0; cli":"=g" (level): :"memory");
    return level;
}

PRIVATE void HAL_IrqRestoreLevel(UArch level)
{
    CASM("pushl %0; popfl": :"g" (level):"memory", "cc");
}

INTERFACE IRQ_Controller IRQ_ControllerInterface = 
{
    .unmask = HAL_IrqUnmask,
    .mask = HAL_IrqMask,
    .ack = HAL_IrqAck,
    .enable = HAL_IrqEnable,
    .disable = HAL_IrqDisable,
    .saveLevel = HAL_IrqSaveLevel,
    .restoreLevel = HAL_IrqRestoreLevel,
};
