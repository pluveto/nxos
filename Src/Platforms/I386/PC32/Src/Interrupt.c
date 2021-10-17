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
#include <Mods/Console/Console.h>
#include <Utils/String.h>
#include <Utils/Memory.h>
#include <HAL.h>
#include <PIC.h>

/* irq table for external interrupt */
PRIVATE IRQ_Info irqInfoTable[MAX_INTR_NR];

PRIVATE char *exceptionName[] = {
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
    "Unknown Exception"
};

PRIVATE OS_Error DefaultExceptionHandler(U32 irq, void *arg)
{
    char *name = irqInfoTable[irq].name;
    Cout("Exception: vector/0x" $x(irq) " Name: " $s(name) " handled!" Endln);

    while (1);
    return OS_EOK;
}

PRIVATE OS_Error DefaultExternalHandler(U32 irq, void *arg)
{
    char *name = irqInfoTable[irq].name;
    Cout("External: vector/0x" $x(irq) " Name: " $s(name) " handled!" Endln);
    return OS_EOK;
}

PUBLIC void CPU_InitInterrupt(void)
{
    int i;
    /* exceptions */
    for (i = 0; i < EXTERNAL_BASE; i++)
    {
        irqInfoTable[i].handler = DefaultExceptionHandler;
        irqInfoTable[i].arg = NULL;
        CopyStr(irqInfoTable[i].name, exceptionName[i]);
    }
    /* external */
    for (i = EXTERNAL_BASE; i < EXTERNAL_BASE + MAX_EXTERNAL_NR; i++)
    {
        irqInfoTable[i].handler = DefaultExternalHandler;
        irqInfoTable[i].arg = NULL;
        CopyStr(irqInfoTable[i].name, "external");
    }
    PIC_Init();
}

PUBLIC void InterruptDispatch(void *stackFrame)
{
    HAL_TrapFrame *frame = (HAL_TrapFrame *) stackFrame;
    U32 vector = frame->vectorNumber;
    void *arg;
    IRQ_Handler handler;

    if (vector < EXCEPTION_BASE || vector >= MAX_INTR_NR)
    {
        Cout("unknown intr vector " $x(vector) Endln);
        return;
    }
    /* call handler with different vector */
    if (vector >= EXCEPTION_BASE && vector < EXCEPTION_BASE + MAX_EXCEPTION_NR)
    {
        /* exception */
        handler = irqInfoTable[vector].handler;
        arg = frame;    /* arg is frame */
        if (handler != NULL)
        {
            if (handler(vector, arg) != OS_EOK)
            {
                DefaultExceptionHandler(vector, arg);
            }
        }
    }
    else if (vector >= EXTERNAL_BASE && vector < EXTERNAL_BASE + MAX_EXTERNAL_NR)
    {
        /* external */
        handler = irqInfoTable[vector].handler;
        arg = irqInfoTable[vector].arg;
        if (handler != NULL)
        {
            handler(vector, arg);
            HAL_IRQAck(vector);
        }
    }
    else if (vector >= SYSCALL_BASE && vector < SYSCALL_BASE + MAX_SYSCALL_NR)
    {
        /* syscall */
    }
    else
    {
        Cout("uninstall intr vector " $x(vector) Endln);
        while (1);
    }
}

INTERFACE OS_Error HAL_IRQInstall(IRQ_Number irqno, IRQ_Handler handler, void *arg, char *name)
{
    if (irqno < EXCEPTION_BASE || irqno >= MAX_INTR_NR)
    {
        return OS_EINVAL;
    }
    irqInfoTable[irqno].handler = handler;
    irqInfoTable[irqno].arg = arg;
    Zero(irqInfoTable[irqno].name, IRQ_NAME_LEN);
    CopyStr(irqInfoTable[irqno].name, name);
    return OS_EOK;
}

INTERFACE OS_Error HAL_IRQUnInstall(IRQ_Number irqno)
{
    if (irqno < EXCEPTION_BASE || irqno >= MAX_INTR_NR)
    {
        return OS_EINVAL;
    }
    irqInfoTable[irqno].handler = NULL;
    irqInfoTable[irqno].arg = NULL;
    return OS_EOK;
}

INTERFACE OS_Error HAL_IRQEnable(IRQ_Number irqno)
{
    if (irqno < EXTERNAL_BASE || irqno >= EXTERNAL_BASE + MAX_EXTERNAL_NR)
    {
        return OS_EINVAL;
    }
    PIC_Enable(irqno - EXTERNAL_BASE);
    return OS_EOK;
}

INTERFACE OS_Error HAL_IRQDisable(IRQ_Number irqno)
{
    if (irqno < EXTERNAL_BASE || irqno >= EXTERNAL_BASE + MAX_EXTERNAL_NR)
    {
        return OS_EINVAL;
    }
    PIC_Disable(irqno - EXTERNAL_BASE);
    return OS_EOK;
}

INTERFACE OS_Error HAL_IRQAck(IRQ_Number irqno)
{
    if (irqno < EXTERNAL_BASE || irqno >= EXTERNAL_BASE + MAX_EXTERNAL_NR)
    {
        return OS_EINVAL;
    }
    PIC_Ack(irqno - EXTERNAL_BASE);
    return OS_EOK;
}

INTERFACE void HAL_InterruptEnable(void)
{
    CASM ("sti");
}

INTERFACE void HAL_InterruptDisable(void)
{
    CASM ("cli");
}
