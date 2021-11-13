/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: mutex lock test 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-13     JasonHu           Init
 */

#include <Sched/Mutex.h>
#include <Mods/UTest/UTest.h>

TEST(MutexInit)
{
    Mutex lock;
    EXPECT_NE(MutexInit(NULL), OS_EOK);
    EXPECT_EQ(MutexInit(&lock), OS_EOK);
    EXPECT_NE(MutexInit(&lock), OS_EOK);
}

TEST(MutexLock)
{
    Mutex lock;
    Mutex lockNoInit;

    EXPECT_EQ(MutexInit(&lock), OS_EOK);

    EXPECT_NE(MutexLock(NULL, TRUE), OS_EOK);
    EXPECT_NE(MutexLock(&lockNoInit, TRUE), OS_EOK);

    EXPECT_EQ(MutexLock(&lock, TRUE), OS_EOK);
    EXPECT_NE(MutexLock(&lock, FALSE), OS_EOK);
}

TEST(MutexUnlock)
{
    Mutex lock;
    Mutex lockNoInit;
    
    EXPECT_EQ(MutexInit(&lock), OS_EOK);

    EXPECT_NE(MutexUnlock(NULL), OS_EOK);
    EXPECT_NE(MutexUnlock(&lockNoInit), OS_EOK);

    EXPECT_EQ(MutexLock(&lock, TRUE), OS_EOK);
    EXPECT_EQ(MutexUnlock(&lock), OS_EOK);
}

TEST(MutexLockAndUnlock)
{
    Mutex lock;
    
    EXPECT_EQ(MutexInit(&lock), OS_EOK);

    int i;
    for (i = 0; i < 12; i++)
    {
        EXPECT_EQ(MutexLock(&lock, TRUE), OS_EOK);
        EXPECT_EQ(MutexUnlock(&lock), OS_EOK);
    }
}

TEST_TABLE(Mutex)
{
    TEST_UNIT(MutexInit),
    TEST_UNIT(MutexLock),
    TEST_UNIT(MutexUnlock),
    TEST_UNIT(MutexLockAndUnlock),
};

TEST_CASE(Mutex);
