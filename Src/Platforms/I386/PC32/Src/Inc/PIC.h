/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Programmable Interrupt Controller 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-16     JasonHu           Init
 */

#ifndef __I386_PIC__
#define __I386_PIC__

#include <XBook.h>

#define PIC_MASTER_CTL		0x20	// I/O port for interrupt controller         <Master>
#define PIC_MASTER_CTLMASK  0x21	// setting bits in this port disables ints   <Master>
#define PIC_SLAVE_CTL	    0xa0	// I/O port for second interrupt controller  <Slave>
#define PIC_SLAVE_CTLMASK	0xa1	// setting bits in this port disables ints   <Slave>

#define PIC_EIO             0x20    /* end of IO port */

PUBLIC void PIC_Init(void);
PUBLIC void PIC_Enable(U32 irq);
PUBLIC void PIC_Disable(U32 irq);
PUBLIC void PIC_Ack(U32 irq);

#endif /* __I386_PIC__ */
