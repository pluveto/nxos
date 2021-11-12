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

#include <Sched/SpinLock.h>
#include <Mods/UTest/UTest.h>

TEST(SpinLockInit)
{
    SpinLock lock;
    EXPECT_NE(SpinLockInit(NULL), OS_EOK);
    EXPECT_EQ(SpinLockInit(&lock), OS_EOK);
    EXPECT_NE(SpinLockInit(&lock), OS_EOK);
}

TEST(SpinLockLock)
{
    SpinLock lock;
    SpinLock lockNoInit;

    EXPECT_EQ(SpinLockInit(&lock), OS_EOK);

    EXPECT_NE(SpinLockLock(NULL), OS_EOK);
    EXPECT_NE(SpinLockLock(&lockNoInit), OS_EOK);

    EXPECT_EQ(SpinLockLock(&lock), OS_EOK);
}

TEST(SpinLockUnlock)
{
    SpinLock lock;
    SpinLock lockNoInit;
    
    EXPECT_EQ(SpinLockInit(&lock), OS_EOK);

    EXPECT_NE(SpinLockUnlock(NULL), OS_EOK);
    EXPECT_NE(SpinLockUnlock(&lockNoInit), OS_EOK);

    EXPECT_EQ(SpinLockLock(&lock), OS_EOK);
    EXPECT_EQ(SpinLockUnlock(&lock), OS_EOK);
}

TEST(SpinLockLockAndUnlock)
{
    SpinLock lock;
    
    EXPECT_EQ(SpinLockInit(&lock), OS_EOK);

    int i;
    for (i = 0; i < 12; i++)
    {
        EXPECT_EQ(SpinLockLock(&lock), OS_EOK);
        EXPECT_EQ(SpinLockUnlock(&lock), OS_EOK);
    }
}

TEST_TABLE(SpinLock)
{
    TEST_UNIT(SpinLockInit),
    TEST_UNIT(SpinLockLock),
    TEST_UNIT(SpinLockUnlock),
    TEST_UNIT(SpinLockLockAndUnlock),
};

TEST_CASE(SpinLock);
