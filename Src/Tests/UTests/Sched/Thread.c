/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: thread utest 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-27     JasonHu           Init
 */

#include <Mods/Test/UTest.h>
#include <Sched/Thread.h>

#ifdef CONFIG_UTEST_SCHED_THREAD

TEST(ThreadSleep)
{
    TimeVal s, e;

    EXPECT_EQ(ThreadSleep(1), OS_EOK);
    EXPECT_EQ(ThreadSleep(10), OS_EOK);
    EXPECT_EQ(ThreadSleep(50), OS_EOK);

    s = ClockTickGetMillisecond();
    EXPECT_EQ(ThreadSleep(100), OS_EOK);
    e = ClockTickGetMillisecond();
    EXPECT_IN_RANGE(e - s, 100, 100 + TICKS_PER_SECOND / (TICKS_PER_SECOND / 10));
    
    s = ClockTickGetMillisecond();
    EXPECT_EQ(ThreadSleep(500), OS_EOK);
    e = ClockTickGetMillisecond();
    EXPECT_IN_RANGE(e - s, 500, 500 + TICKS_PER_SECOND / (TICKS_PER_SECOND / 10));
    
    s = ClockTickGetMillisecond();
    EXPECT_EQ(ThreadSleep(1000), OS_EOK);
    e = ClockTickGetMillisecond();
    EXPECT_IN_RANGE(e - s, 100, 1000 + TICKS_PER_SECOND / (TICKS_PER_SECOND / 10));
    
    s = ClockTickGetMillisecond();
    EXPECT_EQ(ThreadSleep(3000), OS_EOK);
    e = ClockTickGetMillisecond();
    EXPECT_IN_RANGE(e - s, 3000, 3000 + TICKS_PER_SECOND / (TICKS_PER_SECOND / 10));
}

PRIVATE void ThreadSleep1(void *arg)
{
    EXPECT_NE(ThreadSleep(2000), OS_EOK);
}

TEST(ThreadSleepIntr)
{
    
    Thread *thread = ThreadCreate("sleep1", ThreadSleep1, NULL);
    EXPECT_NOT_NULL(thread);
    EXPECT_EQ(ThreadRun(thread), OS_EOK);

    EXPECT_EQ(ThreadSleep(500), OS_EOK);

    /* term thread */
    EXPECT_EQ(ThreadTerminate(thread), OS_EOK);

    EXPECT_EQ(ThreadSleep(100), OS_EOK);
}

TEST_TABLE(Thread)
{
    TEST_UNIT(ThreadSleep),
    TEST_UNIT(ThreadSleepIntr),
};

TEST_CASE(Thread);

#endif
