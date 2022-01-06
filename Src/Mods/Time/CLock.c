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
#define NX_LOG_LEVEL NX_LOG_INFO 
#include <Utils/Log.h>
#include <XBook/Debug.h>

#include <Sched/Thread.h>
#include <Sched/Sched.h>
#include <Sched/SMP.h>

#include <IO/DelayIRQ.h>

#define NX_LOG_NAME "Clock"
#include <Utils/Log.h>

/* NOTE: must add NX_VOLATILE here, avoid compiler optimization  */
NX_PRIVATE NX_VOLATILE NX_ClockTick SystemClockTicks;

NX_PRIVATE NX_IRQ_DelayWork TimerWork;
NX_PRIVATE NX_IRQ_DelayWork SchedWork;

NX_PUBLIC NX_ClockTick NX_ClockTickGet(void)
{
    return SystemClockTicks;
}

NX_PUBLIC void NX_ClockTickSet(NX_ClockTick tick)
{
    SystemClockTicks = tick;
}

NX_PUBLIC void NX_ClockTickGo(void)
{
    /* only boot core change system clock and timer */
    if (NX_SMP_GetBootCore() == NX_SMP_GetIdx())
    {
        SystemClockTicks++;

        NX_IRQ_DelayWorkHandle(&TimerWork);
    }
#ifdef CONFIG_NX_ENABLE_SCHED
    NX_IRQ_DelayWorkHandle(&SchedWork);
#endif
}

NX_PUBLIC NX_Error NX_ClockTickDelay(NX_ClockTick ticks)
{
    NX_ClockTick start = NX_ClockTickGet();
    while (NX_ClockTickGet() - start < ticks)
    {
        /* do nothing to delay */

        /**
         * TODO: add thread state check
         * 
         * if (thread exit flags == 1)
         * {
         *     return NX_ETIMEOUT
         * }
        */
    }
    return NX_EOK; 
}

NX_PRIVATE void NX_TimerIrqHandler(void *arg)
{
    NX_TimerGo();
}

NX_PRIVATE void NX_SchedIrqHandler(void *arg)
{
    NX_Thread *thread = NX_ThreadSelf();
    thread->ticks--;
    if (thread->ticks == 0)
    {
        // NX_LOG_I("thread:%s need sched", thread->name);
        thread->needSched = 1; /* mark sched */
    }
    NX_ASSERT(thread->ticks >= 0);
}

NX_PUBLIC NX_Error NX_ClockInit(void)
{
    NX_Error err;
    err = NX_IRQ_DelayWorkInit(&TimerWork, NX_TimerIrqHandler, NX_NULL, NX_IRQ_WORK_NOREENTER);
    if (err != NX_EOK)
    {
        goto End;
    }
    err = NX_IRQ_DelayWorkInit(&SchedWork, NX_SchedIrqHandler, NX_NULL, NX_IRQ_WORK_NOREENTER);
    if (err != NX_EOK)
    {
        goto End;
    }
    err = NX_IRQ_DelayQueueEnter(NX_IRQ_FAST_QUEUE, &TimerWork);
    if (err != NX_EOK)
    {
        goto End;
    }
    err = NX_IRQ_DelayQueueEnter(NX_IRQ_SCHED_QUEUE, &SchedWork);
    if (err != NX_EOK)
    {
        NX_IRQ_DelayQueueLeave(NX_IRQ_SCHED_QUEUE, &TimerWork);
    }
End:
    return err;
}
