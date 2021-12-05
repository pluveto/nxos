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
#include <Utils/Debug.h>

#include <Sched/Thread.h>
#include <Sched/Sched.h>

#include <IO/DelayIRQ.h>

#define LOG_NAME "Clock"
#include <Utils/Log.h>

/* NOTE: must add VOLATILE here, avoid compiler optimization  */
PRIVATE VOLATILE ClockTick SystemClockTicks;

PRIVATE IRQ_DelayWork TimerWork;
PRIVATE IRQ_DelayWork SchedWork;

PUBLIC ClockTick ClockTickGet(void)
{
    return SystemClockTicks;
}

PUBLIC void ClockTickSet(ClockTick tick)
{
    SystemClockTicks = tick;
}

PUBLIC void ClockTickGo(void)
{
    SystemClockTicks++;
    if ((SystemClockTicks % TICKS_PER_SECOND) == 0)
    {
        //LOG_I("1s");
    }
    
    IRQ_DelayWorkHandle(&TimerWork);
#ifdef CONFIG_ENABLE_SCHED
    IRQ_DelayWorkHandle(&SchedWork);
#endif
}

PUBLIC OS_Error ClockTickDelay(ClockTick ticks)
{
    ClockTick start = ClockTickGet();
    while (ClockTickGet() - start < ticks)
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

PRIVATE void TimerIrqHandler(void *arg)
{
    TimerGo();
}

PRIVATE void SchedIrqHandler(void *arg)
{
    Thread *thread = ThreadSelf();
    thread->ticks--;
    if (thread->ticks == 0)
    {
        // LOG_I("thread:%s need sched", thread->name);
        thread->needSched = 1; /* mark sched */
    }
    ASSERT(thread->ticks >= 0);
}

PUBLIC OS_Error ClockInit(void)
{
    OS_Error err;
    err = IRQ_DelayWorkInit(&TimerWork, TimerIrqHandler, NULL, 0);
    if (err != OS_EOK)
    {
        goto End;
    }
    err = IRQ_DelayWorkInit(&SchedWork, SchedIrqHandler, NULL, IRQ_WORK_NOREENTER);
    if (err != OS_EOK)
    {
        goto End;
    }
    err = IRQ_DelayQueueEnter(IRQ_FAST_QUEUE, &TimerWork);
    if (err != OS_EOK)
    {
        goto End;
    }
    err = IRQ_DelayQueueEnter(IRQ_SCHED_QUEUE, &SchedWork);
    if (err != OS_EOK)
    {
        IRQ_DelayQueueLeave(IRQ_SCHED_QUEUE, &TimerWork);
    }
End:
    return err;
}
