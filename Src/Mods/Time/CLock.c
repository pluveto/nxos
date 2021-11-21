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
#include <Mods/Time/Timer.h>
#define LOG_LEVEL LOG_INFO 
#include <Utils/Log.h>

#include <Sched/Thread.h>
#include <Sched/Sched.h>

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
    if ((systemClockTicks % TICKS_PER_SECOND) == 0)
    {
        // LOG_I("1s");
    }
    TimerGo();

    Thread *thread = ThreadSelf();
    thread->ticks--;
    if (thread->ticks == 0)
    {
        // LOG_I("thread:%s need sched", thread->name);
        thread->needSched = 1; /* mark sched */
    }
}

PUBLIC OS_Error ClockTickDelay(ClockTick ticks)
{
    ClockTick start = ClockTickGet();
    while (ClockTickGet() < start + ticks)
    {
        /* do nothing to delay */

        /**
         * TODO: add thread state check
         * 
         * if (thread exit flags == 1)
         * {
         *     return OS_ETIMEOUT
         * }
        */
    }
    return OS_EOK; 
}
