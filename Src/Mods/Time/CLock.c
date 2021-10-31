/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Clock time
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-31     JasonHu           Init
 */

#include <Mods/Time/Clock.h>
#define LOG_LEVEL LOG_INFO 
#include <Utils/Log.h>

PRIVATE ClockTick systemClockTicks;

PUBLIC ClockTick ClockTickGet(void)
{
    return systemClockTicks;
}

PUBLIC void ClockTickSet(ClockTick tick)
{
    systemClockTicks = tick;
}

PUBLIC void ClockTickGo(void)
{
    systemClockTicks++;
    if ((systemClockTicks % OS_TICKS_PER_SECOND) == 0)
    {
        // LOG_I("1s");
    }
}
