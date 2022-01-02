/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Heap Cache
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-15     JasonHu           Init
 */

#include <Mods/Test/Integration.h>
#include <Mods/Time/Timer.h>
#define LOG_NAME "Timer"
#include <Utils/Log.h>
#include <Sched/Thread.h>

#ifdef CONFIG_TEST_INTEGRATION_TIMER

PRIVATE void TimerHandler1(Timer *timer, void *arg)
{
    LOG_I("Timer#%s timeout\n", (char *)arg);
}

int timerCounter = 0;
PRIVATE void TimerHandler2(Timer *timer, void *arg)
{
    LOG_I("Timer#%s timeout\n", (char *)arg);
    timerCounter++;
    LOG_I("timerCounter:%d\n", timerCounter);
}

INTEGRATION_TEST(Timer)
{
    Timer *timer1 = TimerCreate(10000, TimerHandler1, "1", FALSE);
    if (timer1 == NULL)
    {
        LOG_E("create timer 1 failed!");
        return OS_ERROR;
    }
    if (TimerStart(timer1) != OS_EOK)
    {
        LOG_E("start timer 1 failed!");
        return OS_ERROR;
    }
    LOG_I("start timer 1 OK!");
    
    Timer *timer2 = TimerCreate(1000, TimerHandler2, "2", TRUE);
    if (timer2 == NULL)
    {
        LOG_E("create timer 2 failed!");
        return OS_ERROR;
    }
    if (TimerStart(timer2) != OS_EOK)
    {
        LOG_E("start timer 2 failed!");
        return OS_ERROR;
    }
    LOG_I("start timer 2 OK!");

    Timer *timer3 = TimerCreate(1000, TimerHandler2, "2", TRUE);
    if (timer3 == NULL)
    {
        LOG_E("create timer 2 failed!");
        return OS_ERROR;
    }
    OS_Error err = TimerDestroy(timer3);
    if (err != OS_EOK)
    {
        LOG_E("destroy timer 3 failed!");
        return OS_ERROR;
    }

    int i = 0;
    timerCounter = 0;
    while (1)
    {
        i++;
        if (i % 0xf0000000 == 0)
        {
            LOG_I("???");
        }
#if 1
        if (timerCounter != 3)
        {
            // LOG_I("!!!!!stop timer 2");       
        }
        else
        {
            LOG_I("!!!!!stop timer 2");       
            timerCounter++;
            if (timer2->state != TIMER_STOPPED)
            {
                //TimerStop(timer2);
                //LOG_I("Timer Destroy:%d\n", TimerDestroy(timer2));
            }
        }
#else
        if (timerCounter >= 4)
        {
            LOG_I("timerCounter:%d break", timerCounter);
            break;
        }
#endif
    }
    LOG_I("timerCounter:%d", timerCounter);
    return OS_EOK;
}

#endif
