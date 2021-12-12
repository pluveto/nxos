/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Clock for system 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-16     JasonHu           Init
 */

#include <Mods/Time/Clock.h>
#include <IO/IRQ.h>
#include <IO/DelayIRQ.h>

#include <Clock.h>
#include <Regs.h>
#include <SBI.h>

#define LOG_NAME "Clock"
#include <Utils/Log.h>

#define TIMER_CLK_FREQ 10000000  /* qemu has 10MHZ clock frequency */

PRIVATE U64 TickDelta = TIMER_CLK_FREQ / TICKS_PER_SECOND;

PRIVATE U64 GetTimerCounter()
{
    U64 ret;
    CASM ("rdtime %0" : "=r"(ret));
    return ret;
}

PUBLIC void HAL_ClockHandler(void)
{
    ClockTickGo();
    /* update timer */
    sbi_set_timer(GetTimerCounter() + TickDelta);
}

PUBLIC void HAL_InitClock(void)
{
    /* Clear the Supervisor-Timer bit in SIE */
    ClearCSR(sie, SIE_STIE);

    /* Set timer */
    sbi_set_timer(GetTimerCounter() + TickDelta);

    /* Enable the Supervisor-Timer bit in SIE */
    SetCSR(sie, SIE_STIE);
}
