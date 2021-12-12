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
#include <IO/IRQ.h>

#define PLIC_PHY_ADDR           0x0C000000L

/* interrupt priority */
#define PLIC_PRIORITY           (PLIC_PHY_ADDR + 0x0)
/* interrupt pending */
#define PLIC_PENDING            (PLIC_PHY_ADDR + 0x1000)

/* supervisor interrupt enable for each hart */
#define PLIC_SENABLE(hart)      (PLIC_PHY_ADDR + 0x2080 + (hart) * 0x100)
/* supervisor priority threshold for each hart */
#define PLIC_STHRESHOLD(hart)   (PLIC_PHY_ADDR + 0x201000 + (hart) * 0x2000)
/* supervisor claim for each hart */
#define PLIC_SCLAIM(hart)       (PLIC_PHY_ADDR + 0x201004 + (hart) * 0x2000)

PUBLIC OS_Error PLIC_EnableIRQ(U32 hart, IRQ_Number irqno);
PUBLIC OS_Error PLIC_DisableIRQ(U32 hart, IRQ_Number irqno);
PUBLIC IRQ_Number PLIC_Claim(U32 hart);
PUBLIC OS_Error PLIC_Complete(U32 hart, int irqno);
PUBLIC void PLIC_Init(Bool bootCore);

#endif  /* __RISCV_PLIC__ */
