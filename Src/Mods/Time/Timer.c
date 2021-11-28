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

#include <Mods/Time/Timer.h>
#include <MM/Alloc.h>

#include <Utils/Debug.h>

#define IDLE_TIMER_TIMEOUT  MAX_TIMER_TIMEOUT
#define IDLE_TIMER_TIMEOUT_TICKS  (IDLE_TIMER_TIMEOUT / (1000 / TICKS_PER_SECOND))

PRIVATE LIST_HEAD(TimerListHead);

/* timer tick is different with clock tick */
PRIVATE VOLATILE ClockTick TimerTicks = 0;

/* next timeout tick */
PRIVATE VOLATILE ClockTick NextTimeoutTicks = 0;

PRIVATE Timer IdleTimer;

PUBLIC OS_Error TimerInit(Timer *timer, Uint milliseconds, 
                          void (*handler)(struct Timer *, void *arg), void *arg, 
                          int flags)
{
    if (timer == NULL || !milliseconds || handler == NULL || flags == 0)
    {
        return OS_EINVAL;
    }

    if (!(flags & (TIMER_ONESHOT | TIMER_PERIOD)) || (flags & TIMER_DYNAMIC))
    {
        return OS_EINVAL;
    }

    timer->flags = flags;
    timer->state = TIMER_INITED;

    timer->timeTicks = MILLISECOND_TO_TICKS(milliseconds);
    
    /* calc timeout here */
    timer->timeout = timer->timeTicks + TimerTicks;
    
    timer->handler = handler;
    timer->arg = arg;
    ListInit(&timer->list);
    return OS_EOK;
}

PUBLIC Timer *TimerCreate(Uint milliseconds, 
                          void (*handler)(struct Timer *, void *arg), void *arg, 
                          int flags)
{
    Timer *timer = MemAlloc(sizeof(Timer));
    if (timer == NULL)
    {
        return NULL;
    }
    if (TimerInit(timer, milliseconds, handler, arg, flags) != OS_EOK)
    {
        MemFree(timer);
        return NULL;
    }
    timer->flags |= TIMER_DYNAMIC;
    return timer;
}

PRIVATE void TimerRemove(Timer *timer, Bool onTimerList, Bool destroy)
{
    if (onTimerList == TRUE)
    {
        /* del from list */
        ListDel(&timer->list);

        /* update next time */
        Timer *next;
        ListForEachEntry(next, &TimerListHead, list)
        {
            if (next->timeout > TimerTicks)
            {
                break;
            }
        }
        NextTimeoutTicks = next->timeout;
    }
    if (destroy == TRUE)
    {
        /* free timer */
        if (timer->flags & TIMER_DYNAMIC)
        {
            MemFree(timer);
        }
    }
}

/**
 * destroy a timer, timer must stopped or inited, not waiting and processing.
 */
PUBLIC OS_Error TimerDestroy(Timer *timer)
{
    if (timer == NULL)
    {
        return OS_EINVAL;
    }
    switch (timer->state)
    {
    case TIMER_WAITING:
    case TIMER_PROCESSING:
        return OS_EAGAIN;
    case TIMER_STOPPED:
    case TIMER_INITED:
        TimerRemove(timer, FALSE, TRUE);
        break;
    default:
        return OS_EINVAL;
    }
    return OS_EOK;
}

PUBLIC OS_Error TimerStart(Timer *timer)
{
    if (timer == NULL)
    {
        return OS_EINVAL;
    }
    
    Uint level = IRQ_SaveLevel();

    /* timeout is invalid */
    if (IDLE_TIMER_TIMEOUT_TICKS - timer->timeTicks < TimerTicks)
    {
        IRQ_RestoreLevel(level);
        return OS_EINVAL;
    }

    /* make sure not on the list */
    if (ListFind(&timer->list, &TimerListHead))
    {
        IRQ_RestoreLevel(level);
        return OS_EAGAIN;
    }
    
    /* waiting timeout state */
    timer->state = TIMER_WAITING;
    if (ListEmpty(&TimerListHead))
    {
        /* inseart at head */
        ListAdd(&timer->list, &TimerListHead);
        NextTimeoutTicks = timer->timeout;
    }
    else
    {
        Timer *first = ListFirstEntry(&TimerListHead, Timer, list);
        if (timer->timeout < first->timeout)
        {
            /* insert at head */
            ListAdd(&timer->list, &TimerListHead);
            NextTimeoutTicks = timer->timeout;
        }
        else
        {
            /* insert after nearly timer */
            Timer *prev;
            ListForEachEntry(prev, &TimerListHead, list)
            {
                if (prev->timeout <= timer->timeout)
                {
                    ListAddAfter(&timer->list, &prev->list);
                    break;
                }
            }
        }
    }

    IRQ_RestoreLevel(level);
    return OS_EOK;
}

/**
 * only stop a timer, not destroy
 */
PUBLIC OS_Error TimerStop(Timer *timer)
{
    if (timer == NULL)
    {
        return OS_EINVAL;
    }

    Uint level = IRQ_SaveLevel();
    
    TimerState state = timer->state;

    /* stop must when state is waiting or processing */
    if (state != TIMER_PROCESSING && state != TIMER_WAITING)
    {    
        IRQ_RestoreLevel(level);
        return OS_EAGAIN;
    }

    timer->state = TIMER_STOPPED;

    /* direct del timer when waiting timer */
    if (state == TIMER_WAITING)
    {
        TimerRemove(timer, TRUE, FALSE);
    }

    IRQ_RestoreLevel(level);
    return OS_EOK;
}

PRIVATE void TimerInvoke(Timer *timer)
{
    timer->state = TIMER_PROCESSING;
    timer->handler(timer, timer->arg);

    /* when calling the handler, called stop timer, need stop here */
    if (timer->state == TIMER_STOPPED)
    {
        TimerRemove(timer, TRUE, TRUE);
    }
    else    /* always processing */
    {
        if (timer->flags & TIMER_PERIOD)
        {
            /* update timer timeout */
            timer->timeout = TimerTicks + timer->timeTicks;
            timer->state = TIMER_WAITING;
        }
        else
        {
            timer->state = TIMER_STOPPED;
            TimerRemove(timer, TRUE, TRUE);
        }        
    }
    
}

/**
 * only master cpu will call this
 */
PUBLIC void TimerGo(void)
{
    Timer *timer = NULL;
    Timer *next = NULL;
    
    TimerTicks++;

    if (TimerTicks < NextTimeoutTicks)
    {
        return;
    }

    Uint level = IRQ_SaveLevel();
    
    ListForEachEntrySafe(timer, next, &TimerListHead, list)
    {
        if (timer->timeout > TimerTicks) /* not timeout */
        {
            break;
        }
        /* timeout == TimerTicks -> timeout! */
        TimerInvoke(timer);
    }

    /* find next timer */
    ListForEachEntry(timer, &TimerListHead, list)
    {
        if (timer->timeout > TimerTicks)
        {
            break;
        }
    }
    NextTimeoutTicks = timer->timeout;
    IRQ_RestoreLevel(level);
}

/**
 * recalc all timers timeout
 * this will called with interrupt disabled
 */
PRIVATE void IdleTimerHandler(Timer *timer, void *arg)
{
    ClockTick delta = IdleTimer.timeout;
    TimerTicks -= delta;
    Timer *tmp;
    ListForEachEntry (tmp, &TimerListHead, list)
    {
        tmp->timeout -= delta;
    }
}

PUBLIC void TimersInit(void)
{
    ASSERT(TimerInit(&IdleTimer, IDLE_TIMER_TIMEOUT, IdleTimerHandler, NULL, TRUE) == OS_EOK);
    ASSERT(TimerStart(&IdleTimer) == OS_EOK);
}
