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

#include <I386.h>
#include <PIC.h>
#include <Interrupt.h>

PUBLIC void PIC_Init(void)
{
    /* mask all interrupts */
	IO_Out8(PIC_MASTER_CTLMASK,  0xff);
	IO_Out8(PIC_SLAVE_CTLMASK,   0xff);

	IO_Out8(PIC_MASTER_CTL,      0x11);
	IO_Out8(PIC_MASTER_CTLMASK,  0x20);
	IO_Out8(PIC_MASTER_CTLMASK,  1 << 2);
	IO_Out8(PIC_MASTER_CTLMASK,  0x01);

    IO_Out8(PIC_SLAVE_CTL,       0x11);
	IO_Out8(PIC_SLAVE_CTLMASK,   0x28);
	IO_Out8(PIC_SLAVE_CTLMASK,   2);
	IO_Out8(PIC_SLAVE_CTLMASK,   0x01);

	/* mask all interrupts */
	IO_Out8(PIC_MASTER_CTLMASK,  0xff);
	IO_Out8(PIC_SLAVE_CTLMASK,   0xff);
}

PUBLIC void PIC_Enable(U32 irq)
{
    if(irq < 8){    /* clear master */
        IO_Out8(PIC_MASTER_CTLMASK, IO_In8(PIC_MASTER_CTLMASK) & ~(1 << irq));
    } else {
        /* clear irq 2 first, then clear slave */
        IO_Out8(PIC_MASTER_CTLMASK, IO_In8(PIC_MASTER_CTLMASK) & ~(1 << (IRQ_CONNECT - EXTERNAL_BASE)));    
        IO_Out8(PIC_SLAVE_CTLMASK, IO_In8(PIC_SLAVE_CTLMASK) & ~ (1 << (irq - 8)));
    }
}

PUBLIC void PIC_Disable(U32 irq)   
{
	if(irq < 8){    /* set master */ 
        IO_Out8(PIC_MASTER_CTLMASK, IO_In8(PIC_MASTER_CTLMASK) | (1 << irq));
    } else {
        /* set slave */
        IO_Out8(PIC_SLAVE_CTLMASK, IO_In8(PIC_SLAVE_CTLMASK) | (1 << (irq - 8)));
    }
}

PUBLIC void PIC_Ack(U32 irq)
{
	if (irq >= 8) { /* slaver */
		IO_Out8(PIC_SLAVE_CTL,  PIC_EIO);
	}
	IO_Out8(PIC_MASTER_CTL,  PIC_EIO);
}
