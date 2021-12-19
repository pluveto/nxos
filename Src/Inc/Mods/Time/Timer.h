/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Timer for system
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-20     JasonHu           Init
 */

#ifndef __MODS_TIME_TIMER__
#define __MODS_TIME_TIMER__

#include <XBook.h>
#include <Utils/List.h>
#include <Mods/Time/Clock.h>

/* 24 days */
#define MAX_TIMER_TIMEOUT (24 * 24 * 60 * 60 * 1000)

#define TIMER_ONESHOT   0x01    /* timer type is one shot */
#define TIMER_PERIOD    0x02    /* timer type is period */
#define TIMER_DYNAMIC   0x08    /* timer create from memory heap */

enum TimerState
{
    TIMER_INITED = 1,   /* init, not run */
    TIMER_WAITING,      /* waiting for timeout */
    TIMER_PROCESSING,   /* timer processing */
    TIMER_STOPPED,      /* timer stopped */
};
typedef enum TimerState TimerState;

struct Timer
{
    List list;
    TimerState state;   /* timer state */
    ClockTick timeout;  /* timeout ticks */ 
    ClockTick timeTicks;
    int flags;
    Bool (*handler)(struct Timer *, void *arg);
    void *arg;
};
typedef struct Timer Timer;

PUBLIC OS_Error TimerInit(Timer *timer, Uint milliseconds, 
                          Bool (*handler)(struct Timer *, void *arg), void *arg, 
                          int flags);
                          
PUBLIC Timer *TimerCreate(Uint milliseconds, 
                          Bool (*handler)(struct Timer *, void *arg), void *arg, 
                          int flags);
                          
PUBLIC OS_Error TimerStart(Timer *timer);
PUBLIC OS_Error TimerStop(Timer *timer);
PUBLIC OS_Error TimerDestroy(Timer *timer);

PUBLIC void TimerDump(Timer *timer);

PUBLIC void TimersInit(void);
PUBLIC void TimerGo(void);

#endif  /* __MODS_TIME_TIMER__ */
