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

void IRQ_Disable(void)
{
    S_StatusWrite(S_StatusRead() & ~SSTATUS_SIE);
}

void IRQ_Enable(void)
{
    S_StatusWrite(S_StatusRead() | SSTATUS_SIE);
}
