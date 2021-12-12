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

#include <XBook.h>
#include <PLIC.h>
#include <Regs.h>
#include <IO/IRQ.h>
#include <Utils/Log.h>

PRIVATE OS_Error PLIC_SetPriority(IRQ_Number irqno, U32 priority)
{
    if (irqno <= 0 || irqno >= NR_IRQS)
    {
        return OS_EINVAL;
    }

    /* irq priority must be larger than 0 */
    U32 *priorityReg = (U32 *) PLIC_PRIORITY;
    priorityReg += irqno;
    Write32(priorityReg, priority);

    return OS_EOK;
}

PUBLIC OS_Error PLIC_EnableIRQ(U32 hart, IRQ_Number irqno)
{
    if (hart >= NR_MULTI_CORES || irqno <= 0 || irqno >= NR_IRQS)
    {
        return OS_EINVAL;
    }
    
    /* enable irq */
    U32 *enableReg;
    enableReg = (U32 *)PLIC_SENABLE(hart);
    enableReg += irqno / 32;
    U8 off = irqno % 32;
    Write32(enableReg, Read32(enableReg) | (1 << off));

    PLIC_SetPriority(irqno, 1);

    return OS_EOK;
}

PUBLIC OS_Error PLIC_DisableIRQ(U32 hart, IRQ_Number irqno)
{
    if (hart >= NR_MULTI_CORES || irqno <= 0 || irqno >= NR_IRQS)
    {
        return OS_EINVAL;
    }
    
    /* disable irq */
    U32 *enableReg;
    enableReg = (U32 *)PLIC_SENABLE(hart);
    enableReg += irqno / 32;
    U8 off = irqno % 32;
    Write32(enableReg, Read32(enableReg) & ~(1 << off)); 

    PLIC_SetPriority(irqno, 0);

    return OS_EOK;
}

/**
 * ask the PLIC what interrupt we should serve.
 */
PUBLIC IRQ_Number PLIC_Claim(U32 hart)
{
    if (hart >= NR_MULTI_CORES)
    {
        return 0;
    }

    IRQ_Number irq = *(U32 *)PLIC_SCLAIM(hart);
    return irq;
}

/**
 * tell the PLIC we've served this IRQ.
 */
PUBLIC OS_Error PLIC_Complete(U32 hart, int irqno)
{
    if (hart >= NR_MULTI_CORES || irqno <= 0 || irqno >= NR_IRQS)
    {
        return OS_EINVAL;
    }
    
    *(U32 *)PLIC_SCLAIM(hart) = irqno;
    
    return OS_EOK;
}

PUBLIC void PLIC_Init(Bool bootCore)
{
    if (bootCore == TRUE)
    {
        int hart;
        for (hart = 0; hart < NR_MULTI_CORES; hart++)
        {
            /* priority must be > threshold to trigger an interrupt */
            Write32(PLIC_STHRESHOLD(hart), 0);
            
            /* set all interrupt priority */
            int irqno;
            for (irqno = 1; irqno < NR_IRQS; irqno++)
            {
                PLIC_SetPriority(irqno, 0);
            }
        }
    }
    /* Enable supervisor external interrupts. */
    SetCSR(sie, SIE_SEIE);
}
