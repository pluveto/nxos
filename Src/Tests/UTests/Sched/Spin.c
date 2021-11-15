/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: spin lock test 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-13     JasonHu           Init
 */

#include <Sched/Spin.h>
#include <Mods/Test/UTest.h>

#ifdef CONFIG_UTEST_SCHED_SPIN

TEST(SpinInit)
{
    Spin lock;
    EXPECT_NE(SpinInit(NULL), OS_EOK);
    EXPECT_EQ(SpinInit(&lock), OS_EOK);
    EXPECT_NE(SpinInit(&lock), OS_EOK);
}

TEST(SpinLock)
{
    Spin lock;
    Spin lockNoInit;

    EXPECT_EQ(SpinInit(&lock), OS_EOK);

    EXPECT_NE(SpinLock(NULL, TRUE), OS_EOK);
    EXPECT_NE(SpinLock(&lockNoInit, TRUE), OS_EOK);

    EXPECT_EQ(SpinLock(&lock, TRUE), OS_EOK);
    EXPECT_NE(SpinLock(&lock, FALSE), OS_EOK);
}

TEST(SpinUnlock)
{
    Spin lock;
    Spin lockNoInit;
    
    EXPECT_EQ(SpinInit(&lock), OS_EOK);

    EXPECT_NE(SpinUnlock(NULL), OS_EOK);
    EXPECT_NE(SpinUnlock(&lockNoInit), OS_EOK);

    EXPECT_EQ(SpinLock(&lock, TRUE), OS_EOK);
    EXPECT_EQ(SpinUnlock(&lock), OS_EOK);
}

TEST(SpinLockAndUnlock)
{
    Spin lock;
    
    EXPECT_EQ(SpinInit(&lock), OS_EOK);

    int i;
    for (i = 0; i < 12; i++)
    {
        EXPECT_EQ(SpinLock(&lock, TRUE), OS_EOK);
        EXPECT_EQ(SpinUnlock(&lock), OS_EOK);
    }
}

TEST_TABLE(Spin)
{
    TEST_UNIT(SpinInit),
    TEST_UNIT(SpinLock),
    TEST_UNIT(SpinUnlock),
    TEST_UNIT(SpinLockAndUnlock),
};

TEST_CASE(Spin);

#endif
