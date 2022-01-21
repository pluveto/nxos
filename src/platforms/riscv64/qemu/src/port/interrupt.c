/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: interrupt manage
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-31     JasonHu           Init
 */

#include <xbook.h>
#include <regs.h>
#include <io/irq.h>
#include <plic.h>
#include <sched/smp.h>

NX_PRIVATE NX_Error HAL_IrqUnmask(NX_IRQ_Number irqno)
{
    if (irqno < 0 || irqno >= NX_NR_IRQS)
    {
        return NX_EINVAL;
    }

    return PLIC_EnableIRQ(NX_SMP_GetBootCore(), irqno);
}

NX_PRIVATE NX_Error HAL_IrqMask(NX_IRQ_Number irqno)
{
    if (irqno < 0 || irqno >= NX_NR_IRQS)
    {
        return NX_EINVAL;
    }

    return PLIC_DisableIRQ(NX_SMP_GetBootCore(), irqno);
}

NX_PRIVATE NX_Error HAL_IrqAck(NX_IRQ_Number irqno)
{
    if (irqno < 0 || irqno >= NX_NR_IRQS)
    {
        return NX_EINVAL;
    }
    
    return PLIC_Complete(NX_SMP_GetBootCore(), irqno);
}

NX_PRIVATE void HAL_IrqEnable(void)
{
    WriteCSR(sstatus, ReadCSR(sstatus) | SSTATUS_SIE);
}

NX_PRIVATE void HAL_IrqDisable(void)
{
    WriteCSR(sstatus, ReadCSR(sstatus) & ~SSTATUS_SIE);
}

NX_PRIVATE NX_UArch HAL_IrqSaveLevel(void)
{
    NX_UArch level = 0;
    level = ReadCSR(sstatus);
    WriteCSR(sstatus, level & ~SSTATUS_SIE);
    return level & SSTATUS_SIE;
}

NX_PRIVATE void HAL_IrqRestoreLevel(NX_UArch level)
{
    WriteCSR(sstatus, ReadCSR(sstatus) | (level & SSTATUS_SIE));
}

NX_INTERFACE NX_IRQ_Controller NX_IRQ_ControllerInterface = 
{
    .unmask = HAL_IrqUnmask,
    .mask = HAL_IrqMask,
    .ack = HAL_IrqAck,
    .enable = HAL_IrqEnable,
    .disable = HAL_IrqDisable,
    .saveLevel = HAL_IrqSaveLevel,
    .restoreLevel = HAL_IrqRestoreLevel,
};
