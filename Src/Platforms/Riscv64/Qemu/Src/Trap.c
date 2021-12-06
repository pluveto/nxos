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

PUBLIC void TrapFrameDump(HAL_TrapFrame *frame)
{
    LOG_RAW("------------ Trap frame Dump ------------\n");
    LOG_RAW("Function Registers:\n");
    LOG_RAW("\tra(x1) = 0x%p\n", frame->ra);
    LOG_RAW("\tuser_sp(x2) = 0x%p\n", frame->sp);
    LOG_RAW("\tgp(x3) = 0x%p\n", frame->gp);
    LOG_RAW("\ttp(x4) = 0x%p\n", frame->tp);
    LOG_RAW("Temporary Registers:\n");
    LOG_RAW("\tt0(x5) = 0x%p\n", frame->t0);
    LOG_RAW("\tt1(x6) = 0x%p\n", frame->t1);
    LOG_RAW("\tt2(x7) = 0x%p\n", frame->t2);
    LOG_RAW("\tt3(x28) = 0x%p\n", frame->t3);
    LOG_RAW("\tt4(x29) = 0x%p\n", frame->t4);
    LOG_RAW("\tt5(x30) = 0x%p\n", frame->t5);
    LOG_RAW("\tt6(x31) = 0x%p\n", frame->t6);
    LOG_RAW("Saved Registers:\n");
    LOG_RAW("\ts0/fp(x8) = 0x%p\n", frame->s0);
    LOG_RAW("\ts1(x9) = 0x%p\n", frame->s1);
    LOG_RAW("\ts2(x18) = 0x%p\n", frame->s2);
    LOG_RAW("\ts3(x19) = 0x%p\n", frame->s3);
    LOG_RAW("\ts4(x20) = 0x%p\n", frame->s4);
    LOG_RAW("\ts5(x21) = 0x%p\n", frame->s5);
    LOG_RAW("\ts6(x22) = 0x%p\n", frame->s6);
    LOG_RAW("\ts7(x23) = 0x%p\n", frame->s7);
    LOG_RAW("\ts8(x24) = 0x%p\n", frame->s8);
    LOG_RAW("\ts9(x25) = 0x%p\n", frame->s9);
    LOG_RAW("\ts10(x26) = 0x%p\n", frame->s10);
    LOG_RAW("\ts11(x27) = 0x%p\n", frame->s11);
    LOG_RAW("Function Arguments Registers:\n");
    LOG_RAW("\ta0(x10) = 0x%p\n", frame->a0);
    LOG_RAW("\ta1(x11) = 0x%p\n", frame->a1);
    LOG_RAW("\ta2(x12) = 0x%p\n", frame->a2);
    LOG_RAW("\ta3(x13) = 0x%p\n", frame->a3);
    LOG_RAW("\ta4(x14) = 0x%p\n", frame->a4);
    LOG_RAW("\ta5(x15) = 0x%p\n", frame->a5);
    LOG_RAW("\ta6(x16) = 0x%p\n", frame->a6);
    LOG_RAW("\ta7(x17) = 0x%p\n", frame->a7);
    LOG_RAW("sstatus = 0x%p\n", frame->sstatus);
    LOG_RAW("\t%s\n", (frame->sstatus & SSTATUS_SIE) ? "Supervisor Interrupt Enabled" : "Supervisor Interrupt Disabled");
    LOG_RAW("\t%s\n", (frame->sstatus & SSTATUS_SPIE) ? "Last Time Supervisor Interrupt Enabled" : "Last Time Supervisor Interrupt Disabled");
    LOG_RAW("\t%s\n", (frame->sstatus & SSTATUS_SPP) ? "Last Privilege is Supervisor Mode" : "Last Privilege is User Mode");
    LOG_RAW("\t%s\n", (frame->sstatus & SSTATUS_FS) ? "FPU Enabled" : "FPU not Enabled");
    
    Size satp = ReadCSR(satp);
    LOG_RAW("satp = 0x%p\n", satp);
    
    LOG_RAW("\tCurrent PPN = 0x%p\n", (satp & ((1UL << 28) - 1)));
    LOG_RAW("\tCurrent ASID = 0x%p\n", (satp >> 44) & ((1UL << 16) - 1));

    const char *mmuMode;
    switch ((satp >> 60) & 0xF)
    {
    case 0:
        mmuMode = "Bare - No translation or protection";
        break;
    case 8:
        mmuMode = "Sv39 - Page-based 39-bit virtual addressing";
        break;
    case 9:
        mmuMode = "Sv48 - Page-based 48-bit virtual addressing";
        break;
    case 10:
        mmuMode = "Sv57 - Page-based 57-bit virtual addressing";
        break;
    case 11:
        mmuMode = "Sv64 - Page-based 64-bit virtual addressing";
        break;
    default:
        mmuMode = " - Reserved";
        break;
    }

    LOG_RAW("\tAddress Transport Mode = %s\n", mmuMode);
    LOG_RAW("------------ Trap frame Dump Done ------------\n");
}

PUBLIC void CPU_InitTrap(void)
{
    /* set trap entry */
    WriteCSR(stvec, TrapEntry);

    /* Enable soft interrupt */
    SetCSR(sie, SIE_SSIE);
}

PUBLIC void TrapDispatch(HAL_TrapFrame *frame)
{
    // LOG_D("trap dispatcher");
    U64 cause = ReadCSR(scause);
    U64 stval = ReadCSR(stval);

    const char *msg = NULL;
    Uint id = cause & ((1UL << (RISCV_XLEN - 2)) - 1);

    /* supervisor external interrupt */
    if ((SCAUSE_INTERRUPT & cause) && 
        SCAUSE_S_EXTERNAL_INTR == (cause & 0xff))
    {
        IRQ_Number irqno = PLIC_Claim(0);
        if (irqno != 0)
        {
            IRQ_Handle(irqno);
        }
        return;
    }
    else if ((SCAUSE_INTERRUPT | SCAUSE_S_TIMER_INTR) == cause)
    {
        // LOG_D("supervisor timer");
        /* supervisor timer */
        HAL_ClockHandler();
        return;
    }
    else if (SCAUSE_INTERRUPT & cause)
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

    LOG_E("scause:0x%p, stval:0x%p, sepc:0x%p", cause, stval, frame->epc);
    TrapFrameDump(frame);
    while(1);
}
