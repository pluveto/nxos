/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Platform-Level Interrupt Controller(PLIC)
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-4      JasonHu           Init
 */

#ifndef __RISCV_PLIC__
#define __RISCV_PLIC__

#include <XBook.h>
#include <Riscv.h>
#include <IO/IRQ.h>

#define PLIC_ADDR               RISCV_PLIC_PADDR

/* interrupt priority */
#define PLIC_PRIORITY           (PLIC_ADDR + 0x0)
/* interrupt pending */
#define PLIC_PENDING            (PLIC_ADDR + 0x1000)

/* supervisor interrupt enable for each hart */
#define PLIC_SENABLE(hart)      (PLIC_ADDR + 0x2080 + (hart) * 0x100)
/* supervisor priority threshold for each hart */
#define PLIC_STHRESHOLD(hart)   (PLIC_ADDR + 0x201000 + (hart) * 0x2000)
/* supervisor claim for each hart */
#define PLIC_SCLAIM(hart)       (PLIC_ADDR + 0x201004 + (hart) * 0x2000)

NX_PUBLIC NX_Error PLIC_EnableIRQ(NX_U32 hart, NX_IRQ_Number irqno);
NX_PUBLIC NX_Error PLIC_DisableIRQ(NX_U32 hart, NX_IRQ_Number irqno);
NX_PUBLIC NX_IRQ_Number PLIC_Claim(NX_U32 hart);
NX_PUBLIC NX_Error PLIC_Complete(NX_U32 hart, int irqno);
NX_PUBLIC void PLIC_Init(NX_Bool bootCore);

#endif  /* __RISCV_PLIC__ */
