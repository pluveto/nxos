/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: X86 Gate 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-9-17      JasonHu           Init
 */

#include <Gate.h>
#include <Segment.h>
#include <Interrupt.h>

struct CPU_Gate
{
    u16 offsetLow, selector;
    u8 datacount;
    u8 attributes;      /* P(1) DPL(2) DT(1) TYPE(4) */
    u16 offsetHigh;
};

typedef void (*TrapHandler)(void);

IMPORT void CPU_TrapEntry0x00(void);
IMPORT void CPU_TrapEntry0x01(void);
IMPORT void CPU_TrapEntry0x02(void);
IMPORT void CPU_TrapEntry0x03(void);
IMPORT void CPU_TrapEntry0x04(void);
IMPORT void CPU_TrapEntry0x05(void);
IMPORT void CPU_TrapEntry0x06(void);
IMPORT void CPU_TrapEntry0x07(void);
IMPORT void CPU_TrapEntry0x08(void);
IMPORT void CPU_TrapEntry0x09(void);
IMPORT void CPU_TrapEntry0x0a(void);
IMPORT void CPU_TrapEntry0x0b(void);
IMPORT void CPU_TrapEntry0x0c(void);
IMPORT void CPU_TrapEntry0x0d(void);
IMPORT void CPU_TrapEntry0x0e(void);
IMPORT void CPU_TrapEntry0x0f(void);
IMPORT void CPU_TrapEntry0x10(void);
IMPORT void CPU_TrapEntry0x11(void);
IMPORT void CPU_TrapEntry0x12(void);
IMPORT void CPU_TrapEntry0x13(void);
IMPORT void CPU_TrapEntry0x14(void);
IMPORT void CPU_TrapEntry0x15(void);
IMPORT void CPU_TrapEntry0x16(void);
IMPORT void CPU_TrapEntry0x17(void);
IMPORT void CPU_TrapEntry0x18(void);
IMPORT void CPU_TrapEntry0x19(void);
IMPORT void CPU_TrapEntry0x1a(void);
IMPORT void CPU_TrapEntry0x1b(void);
IMPORT void CPU_TrapEntry0x1c(void);
IMPORT void CPU_TrapEntry0x1d(void);
IMPORT void CPU_TrapEntry0x1e(void);
IMPORT void CPU_TrapEntry0x1f(void);

IMPORT void CPU_TrapEntry0x20(void);
IMPORT void CPU_TrapEntry0x21(void);
IMPORT void CPU_TrapEntry0x22(void);
IMPORT void CPU_TrapEntry0x23(void);
IMPORT void CPU_TrapEntry0x24(void);
IMPORT void CPU_TrapEntry0x25(void);
IMPORT void CPU_TrapEntry0x26(void);
IMPORT void CPU_TrapEntry0x27(void);
IMPORT void CPU_TrapEntry0x28(void);
IMPORT void CPU_TrapEntry0x29(void);
IMPORT void CPU_TrapEntry0x2a(void);
IMPORT void CPU_TrapEntry0x2b(void);
IMPORT void CPU_TrapEntry0x2c(void);
IMPORT void CPU_TrapEntry0x2d(void);
IMPORT void CPU_TrapEntry0x2e(void);
IMPORT void CPU_TrapEntry0x2f(void);

IMPORT void CPU_SyscallEntry(void);
IMPORT void CPU_LoadIDT(ubase size, ubase idtr);    

PRIVATE void SetGate(struct CPU_Gate *gate, TrapHandler handler,
                     u32 selector, u32 attributes, u8 privilege)
{
    ubase offset = (ubase) handler;
    gate->offsetLow  = offset & 0xffff;
    gate->selector    = selector;
    gate->attributes  = attributes | (privilege << 5);
    gate->datacount   = 0;
    gate->offsetHigh = (offset >> 16) & 0xffff;
}

PUBLIC void CPU_InitGate(void)
{
    struct CPU_Gate *idt = (struct CPU_Gate *) (IDT_VADDR);

    int i;
    for (i = 0; i < MAX_IDT_NR; i++) {
        SetGate(OFF(idt, i), 0, 0, 0, 0);
    }
    SetGate(OFF(idt, ERQ_BASE), CPU_TrapEntry0x00, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+1), CPU_TrapEntry0x01, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+2), CPU_TrapEntry0x02, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+3), CPU_TrapEntry0x03, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+4), CPU_TrapEntry0x04, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+5), CPU_TrapEntry0x05, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+6), CPU_TrapEntry0x06, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+7), CPU_TrapEntry0x07, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+8), CPU_TrapEntry0x08, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+9), CPU_TrapEntry0x09, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+10), CPU_TrapEntry0x0a, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+11), CPU_TrapEntry0x0b, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+12), CPU_TrapEntry0x0c, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+13), CPU_TrapEntry0x0d, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+14), CPU_TrapEntry0x0e, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+15), CPU_TrapEntry0x0f, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+16), CPU_TrapEntry0x10, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+17), CPU_TrapEntry0x11, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+18), CPU_TrapEntry0x12, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+19), CPU_TrapEntry0x13, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+20), CPU_TrapEntry0x14, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+21), CPU_TrapEntry0x15, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+22), CPU_TrapEntry0x16, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+23), CPU_TrapEntry0x17, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+24), CPU_TrapEntry0x18, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+25), CPU_TrapEntry0x19, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+26), CPU_TrapEntry0x1a, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+27), CPU_TrapEntry0x1b, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+28), CPU_TrapEntry0x1c, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+29), CPU_TrapEntry0x1d, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+30), CPU_TrapEntry0x1e, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, ERQ_BASE+31), CPU_TrapEntry0x1f, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);

    SetGate(OFF(idt, IRQ_BASE), CPU_TrapEntry0x20, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+1), CPU_TrapEntry0x21, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+2), CPU_TrapEntry0x22, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+3), CPU_TrapEntry0x23, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+4), CPU_TrapEntry0x24, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+5), CPU_TrapEntry0x25, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+6), CPU_TrapEntry0x26, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+7), CPU_TrapEntry0x27, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+8), CPU_TrapEntry0x28, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+9), CPU_TrapEntry0x29, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+10), CPU_TrapEntry0x2a, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+11), CPU_TrapEntry0x2b, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+12), CPU_TrapEntry0x2c, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+13), CPU_TrapEntry0x2d, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+14), CPU_TrapEntry0x2e, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);
    SetGate(OFF(idt, IRQ_BASE+15), CPU_TrapEntry0x2f, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL0);

    SetGate(OFF(idt, SYSCALL_BASE), CPU_SyscallEntry, KERNEL_CODE_SEL, DA_386_INTR_GATE, DA_GATE_DPL3);
    
    CPU_LoadIDT(IDT_LIMIT, IDT_VADDR);
}
