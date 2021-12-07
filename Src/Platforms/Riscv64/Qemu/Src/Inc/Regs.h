/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: arch registers 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-31     JasonHu           Init
 */

#ifndef __PLATFROM_REGS__
#define __PLATFROM_REGS__

#include <XBook.h>

#define SSTATUS_SPP     (1L << 8)  // Previous mode, 1=Supervisor, 0=User
#define SSTATUS_SPIE    (1L << 5) // Supervisor Previous Interrupt Enable
#define SSTATUS_UPIE    (1L << 4) // User Previous Interrupt Enable
#define SSTATUS_SIE     (1L << 1)  // Supervisor Interrupt Enable
#define SSTATUS_UIE     (1L << 0)  // User Interrupt Enable
#define SSTATUS_FS      (1L << 13)  // Float support
#define SSTATUS_XS      (1L << 14)  // Accelerator support
#define SSTATUS_SUM     (1L << 18)  // Supervisor Access User memroy

#define RISCV_XLEN    64

#define SCAUSE_INTERRUPT        (1UL << (RISCV_XLEN - 1))
#define SCAUSE_S_SOFTWARE_INTR  1
#define SCAUSE_S_TIMER_INTR     5
#define SCAUSE_S_EXTERNAL_INTR  9

#define IRQ_S_SOFT  1
#define IRQ_H_SOFT  2
#define IRQ_M_SOFT  3
#define IRQ_S_TIMER 5
#define IRQ_H_TIMER 6
#define IRQ_M_TIMER 7
#define IRQ_S_EXT   9
#define IRQ_H_EXT   10
#define IRQ_M_EXT   11
#define IRQ_COP     12
#define IRQ_HOST    13

#define SIE_SSIE    (1 << IRQ_S_SOFT)
#define SIE_STIE    (1 << IRQ_S_TIMER)
#define SIE_SEIE    (1 << IRQ_S_EXT)

#ifndef __ASSEMBLY__
/* csr registers read/write */
#define ReadCSR(reg) ({ U64 __tmp; \
    CASM ("csrr %0, " #reg : "=r"(__tmp)); \
    __tmp; })

#define WriteCSR(reg, val) ({ \
    if (__builtin_constant_p(val) && (U64)(val) < 32) \
        CASM ("csrw " #reg ", %0" :: "i"(val)); \
    else \
        CASM ("csrw " #reg ", %0" :: "r"(val)); })

#define SwapCSR(reg, val) ({ U64 __tmp; \
    if (__builtin_constant_p(val) && (U64)(val) < 32) \
        CASM ("csrrw %0, " #reg ", %1" : "=r"(__tmp) : "i"(val)); \
    else \
        CASM ("csrrw %0, " #reg ", %1" : "=r"(__tmp) : "r"(val)); \
    __tmp; })

#define SetCSR(reg, bit) ({ U64 __tmp; \
    if (__builtin_constant_p(bit) && (U64)(bit) < 32) \
        CASM ("csrrs %0, " #reg ", %1" : "=r"(__tmp) : "i"(bit)); \
    else \
        CASM ("csrrs %0, " #reg ", %1" : "=r"(__tmp) : "r"(bit)); \
    __tmp; })

#define ClearCSR(reg, bit) ({ U64 __tmp; \
    if (__builtin_constant_p(bit) && (U64)(bit) < 32) \
        CASM ("csrrc %0, " #reg ", %1" : "=r"(__tmp) : "i"(bit)); \
    else \
        CASM ("csrrc %0, " #reg ", %1" : "=r"(__tmp) : "r"(bit)); \
    __tmp; })

/* read/write memory */
#define Read8(addr) (*(VOLATILE U8 *)(addr))
#define Read16(addr) (*(VOLATILE U16 *)(addr))
#define Read32(addr) (*(VOLATILE U32 *)(addr))
#define Read64(addr) (*(VOLATILE U64 *)(addr))

#define Write8(addr, value)                      \
    {                                        \
        (*(VOLATILE U8 *)(addr)) = (value); \
    }
#define Write16(addr, value)                       \
    {                                         \
        (*(VOLATILE U16 *)(addr)) = (value); \
    }
#define Write32(addr, value)                       \
    {                                         \
        (*(VOLATILE U32 *)(addr)) = (value); \
    }
#define Write64(addr, value)                       \
    {                                         \
        (*(VOLATILE U64 *)(addr)) = (value); \
    }

#endif /* !__ASSEMBLY__ */

#endif  /* __PLATFROM_REGS__ */
