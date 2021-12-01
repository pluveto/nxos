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

#include <XBook.h>
#include <XBook/HAL.h>
#include <Regs.h>
#include <IO/IRQ.h>

PRIVATE OS_Error HAL_IrqUnmask(IRQ_Number irqno)
{
    if (irqno < 0 || irqno >= NR_IRQS)
    {
        return OS_EINVAL;
    }

    return OS_EOK;
}

PRIVATE OS_Error HAL_IrqMask(IRQ_Number irqno)
{
    if (irqno < 0 || irqno >= NR_IRQS)
    {
        return OS_EINVAL;
    }
    return OS_EOK;
}

PRIVATE OS_Error HAL_IrqAck(IRQ_Number irqno)
{
    if (irqno < 0 || irqno >= NR_IRQS)
    {
        return OS_EINVAL;
    }
    return OS_EOK;
}

PRIVATE void HAL_IrqEnable(void)
{
    SstatusWrite(SstatusRead() | SSTATUS_SIE);
}

PRIVATE void HAL_IrqDisable(void)
{
    SstatusWrite(SstatusRead() & ~SSTATUS_SIE);
}

PRIVATE Uint HAL_IrqSaveLevel(void)
{
    Uint level = 0;
    level = SstatusRead();
    SstatusWrite(level & ~SSTATUS_SIE);
    return level & SSTATUS_SIE;
}

PRIVATE void HAL_IrqRestoreLevel(Uint level)
{
    SstatusWrite(SstatusRead() | (level & SSTATUS_SIE));
}

INTERFACE IRQ_Controller IRQ_ControllerInterface = 
{
    .unmask = HAL_IrqUnmask,
    .mask = HAL_IrqMask,
    .ack = HAL_IrqAck,
    .enable = HAL_IrqEnable,
    .disable = HAL_IrqDisable,
    .saveLevel = HAL_IrqSaveLevel,
    .restoreLevel = HAL_IrqRestoreLevel,
};
