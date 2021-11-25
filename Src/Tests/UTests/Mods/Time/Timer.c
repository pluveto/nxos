/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: utest self test 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-2      JasonHu           Init
 */

#include <Mods/Test/UTest.h>

#include <Mods/Time/Timer.h>

#ifdef CONFIG_UTEST_MODS_UTEST

PRIVATE int TimerOneshotFlags = 0;

PRIVATE void TimerHandler(Timer *timer, void *arg)
{
    TimerOneshotFlags++;
    if (arg == (void *)0x1234abcd)
    {
        TimerOneshotFlags++;    
    }
}

#define TIMER_PERIOD_COUNT 10
PRIVATE int TimerPeriodFlags = 0;
PRIVATE void TimerHandler2(Timer *timer, void *arg)
{
    TimerPeriodFlags++;
    if (TimerPeriodFlags == TIMER_PERIOD_COUNT)
    {
        TimerStop(timer);
    }
}

TEST(TimerCreateAndDestroy)
{
    Timer *timer0 = TimerCreate(1000, TimerHandler, (void *)0x1234abcd, FALSE);
    EXPECT_NOT_NULL(timer0);

    Timer *timer1 = TimerCreate(1000, TimerHandler, (void *)0x1234abcd, FALSE);
    EXPECT_NOT_NULL(timer1);

    Timer *timer2 = TimerCreate(0, TimerHandler, (void *)0x1234abcd, FALSE);
    EXPECT_NULL(timer2);

    Timer *timer3 = TimerCreate(10, NULL, (void *)0x1234abcd, FALSE);
    EXPECT_NULL(timer3);

    Timer *timer4 = TimerCreate(10, TimerHandler, NULL, TRUE);
    EXPECT_NOT_NULL(timer4);

    EXPECT_NE(TimerDestroy(NULL), OS_EOK);
    EXPECT_EQ(TimerDestroy(timer0), OS_EOK);
    EXPECT_EQ(TimerDestroy(timer1), OS_EOK);
    EXPECT_NE(TimerDestroy(timer2), OS_EOK);
    EXPECT_NE(TimerDestroy(timer3), OS_EOK);
    EXPECT_EQ(TimerDestroy(timer4), OS_EOK);
}

TEST(TimerStart)
{
    TimerOneshotFlags = 0;
    EXPECT_NE(TimerStart(NULL), OS_EOK);
    Timer *timer0 = TimerCreate(1000, TimerHandler, (void *)0x1234abcd, FALSE);
    EXPECT_NOT_NULL(timer0);
    EXPECT_EQ(TimerStart(timer0), OS_EOK);
    ClockTickDelayMilliSecond(1500);
    EXPECT_EQ(TimerOneshotFlags, 2);

    TimerPeriodFlags = 0;
    Timer *timer1 = TimerCreate(100, TimerHandler2, (void *)0x1234abcd, TRUE);
    EXPECT_NOT_NULL(timer1);
    EXPECT_EQ(TimerStart(timer1), OS_EOK);
    ClockTickDelayMilliSecond(1500);
    EXPECT_EQ(TimerPeriodFlags, TIMER_PERIOD_COUNT);
}

TEST_TABLE(Timer)
{
    TEST_UNIT(TimerCreateAndDestroy),
    TEST_UNIT(TimerStart),
};

TEST_CASE(Timer);

#endif
