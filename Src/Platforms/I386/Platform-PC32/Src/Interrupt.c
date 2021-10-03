/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: interrupt
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

#include <Gate.h>
#include <Interrupt.h>
#include <Mods/Console/Console.h>

void InterruptDispatch(void *stackFrame)
{
    COUT Str("Interrupt Occur!\n");
}
