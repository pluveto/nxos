/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Clock manage
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-31     JasonHu           Init
 */

#ifndef __MODS_TIME_CLOCK__
#define __MODS_TIME_CLOCK__

#include <XBook.h>

#ifdef CONFIG_TICKS_PER_SECOND
#define TICKS_PER_SECOND CONFIG_TICKS_PER_SECOND
#else
#define TICKS_PER_SECOND 100
#endif

#if (TICKS_PER_SECOND > 1000) || (TICKS_PER_SECOND == 0)
#error "config ticks range in [1~1000]"
#endif

#define TICKS_PER_MILLISECOND (1000 / TICKS_PER_SECOND)
#define MILLISECOND_TO_TICKS(msec) ((msec) / TICKS_PER_MILLISECOND)
#define TICKS_TO_MILLISECOND(ticks) ((ticks) * TICKS_PER_MILLISECOND)

typedef Uint TimeVal;
typedef Uint ClockTick;

PUBLIC ClockTick ClockTickGet(void);
PUBLIC void ClockTickSet(ClockTick tick);
PUBLIC void ClockTickGo(void);

PUBLIC OS_Error ClockTickDelay(ClockTick ticks);

INLINE TimeVal ClockTickToMilliSecond(ClockTick tick)
{
    return TICKS_TO_MILLISECOND(tick);
}

INLINE ClockTick MilliSecondToClockTick(TimeVal milliSecond)
{
    return MILLISECOND_TO_TICKS(milliSecond);
}

INLINE ClockTick ClockTickGetMilliSecond(void)
{
    return ClockTickToMilliSecond(ClockTickGet());
}

INLINE ClockTick ClockTickDelayMilliSecond(TimeVal milliSecond)
{
    return ClockTickDelay(MilliSecondToClockTick(milliSecond));
}

#endif  /* __MODS_TIME_CLOCK__ */
