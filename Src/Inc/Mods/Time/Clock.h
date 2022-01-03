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

typedef UArch TimeVal;
typedef UArch ClockTick;

PUBLIC ClockTick ClockTickGet(void);
PUBLIC void ClockTickSet(ClockTick tick);
PUBLIC void ClockTickGo(void);

PUBLIC OS_Error ClockTickDelay(ClockTick ticks);

INLINE TimeVal ClockTickToMillisecond(ClockTick tick)
{
    return TICKS_TO_MILLISECOND(tick);
}

INLINE ClockTick MillisecondToClockTick(TimeVal milliseconds)
{
    return MILLISECOND_TO_TICKS(milliseconds);
}

INLINE TimeVal ClockTickGetMillisecond(void)
{
    return ClockTickToMillisecond(ClockTickGet());
}

INLINE OS_Error ClockTickDelayMillisecond(TimeVal milliseconds)
{
    return ClockTickDelay(MillisecondToClockTick(milliseconds));
}

PUBLIC OS_Error ClockInit(void);

#endif  /* __MODS_TIME_CLOCK__ */
