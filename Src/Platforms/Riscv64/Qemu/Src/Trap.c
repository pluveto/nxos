/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: trap init
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-3      JasonHu           Init
 */

#include <Regs.h>
#include <Trap.h>
#include <Interrupt.h>
#include <Clock.h>
#include <PLIC.h>

#define LOG_NAME "Trap"
#include <Utils/Log.h>

IMPORT void TrapEntry();

/* trap name for riscv */
PRIVATE const char *InterruptName[] =
{
    "User Software Interrupt",
    "Supervisor Software Interrupt",
    "Reversed-2",
    "Reversed-3",
    "User Timer Interrupt",
    "Supervisor Timer Interrupt",
    "Reversed-6",
    "Reversed-7",
    "User External Interrupt",
    "Supervisor External Interrupt",
    "Reserved-10",
    "Reserved-11",
};

PRIVATE const char *ExceptionName[] =
{
    "Instruction Address Misaligned",
    "Instruction Access Fault",
    "Illegal Instruction",
    "Breakpoint",
    "Load Address Misaligned",
    "Load Access Fault",
    "Store/AMO Address Misaligned",
    "Store/AMO Access Fault",
    "Environment call from U-mode",
    "Environment call from S-mode",
    "Reserved-10",
    "Reserved-11",
    "Instruction Page Fault",
    "Load Page Fault",
    "Reserved-14",
    "Store/AMO Page Fault"
};

PUBLIC void CPU_InitTrap(void)
{
    /**
     * if sscratch == 0 : interrupt from kernel mode
     * else             : interrupt from user mode
     */
    WriteCSR(sscratch, 0);
    
    /* set trap entry */
    WriteCSR(stvec, TrapEntry);

    /* Enable soft interrupt */
    SetCSR(sie, SIE_SSIE);
}

PUBLIC void TrapDispatch(HAL_TrapFrame *frame)
{
    // LOG_D("trap dispatcher");
    const char *msg = NULL;
    Uint id = frame->cause & ((1UL << (RISCV_XLEN - 2)) - 1);

    /* supervisor external interrupt */
    if ((SCAUSE_INTERRUPT & frame->cause) && 
        SCAUSE_S_EXTERNAL_INTR == (frame->cause & 0xff))
    {
        IRQ_Number irqno = PLIC_Claim(0);
        if (irqno != 0)
        {
            IRQ_Handle(irqno);
        }
        return;
    }
    else if ((SCAUSE_INTERRUPT | SCAUSE_S_TIMER_INTR) == frame->cause)
    {
        // LOG_D("supervisor timer");
        /* supervisor timer */
        HAL_ClockHandler();
        return;
    }
    else if (SCAUSE_INTERRUPT & frame->cause)
    {
        if(id < sizeof(InterruptName) / sizeof(const char *))
        {
            msg = InterruptName[id];
        }
        else
        {
            msg = "Unknown Interrupt";
        }
        LOG_E("Unhandled Interrupt %ld:%s", id, msg);
    }
    else
    {
        if(id < sizeof(ExceptionName) / sizeof(const char *))
        {
            msg = ExceptionName[id];
        }
        else
        {
            msg = "Unknown Exception";
        }
        LOG_E("Unhandled Exception %ld:%s", id, msg);
    }

    LOG_E("scause:0x%p, stval:0x%p, sepc:0x%p", frame->cause, frame->tval, frame->epc);
    while(1);
}
