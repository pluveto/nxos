/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: atomic test 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-12     JasonHu           Init
 */

#include <XBook/Atomic.h>
#include <Mods/Test/UTest.h>

#ifdef CONFIG_UTEST_XBOOK_ATOMIC

TEST(AtomicSetAndGet)
{
    HAL_Atomic val;
    HAL_AtomicSet(&val, 0);
    EXPECT_EQ(HAL_AtomicGet(&val), 0);
    
    HAL_AtomicSet(&val, 1);
    EXPECT_EQ(HAL_AtomicGet(&val), 1);
}

TEST(AtomicAdd)
{
    HAL_Atomic val;
    HAL_AtomicSet(&val, 0);
    HAL_AtomicAdd(&val, 1);
    EXPECT_EQ(HAL_AtomicGet(&val), 1);
    HAL_AtomicAdd(&val, 3);
    EXPECT_EQ(HAL_AtomicGet(&val), 4);
    HAL_AtomicAdd(&val, 100);
    EXPECT_EQ(HAL_AtomicGet(&val), 104);
}

TEST(AtomicSub)
{
    HAL_Atomic val;
    HAL_AtomicSet(&val, 0);
    HAL_AtomicSub(&val, 1);
    EXPECT_EQ(HAL_AtomicGet(&val), -1);
    HAL_AtomicSub(&val, 3);
    EXPECT_EQ(HAL_AtomicGet(&val), -4);
    HAL_AtomicSub(&val, 100);
    EXPECT_EQ(HAL_AtomicGet(&val), -104);
}

TEST(AtomicInc)
{
    HAL_Atomic val;
    HAL_AtomicSet(&val, 0);
    HAL_AtomicInc(&val);
    EXPECT_EQ(HAL_AtomicGet(&val), 1);
    HAL_AtomicInc(&val);
    EXPECT_EQ(HAL_AtomicGet(&val), 2);
    HAL_AtomicInc(&val);
    EXPECT_EQ(HAL_AtomicGet(&val), 3);
}

TEST(AtomicDec)
{
    HAL_Atomic val;
    HAL_AtomicSet(&val, 0);
    HAL_AtomicDec(&val);
    EXPECT_EQ(HAL_AtomicGet(&val), -1);
    HAL_AtomicDec(&val);
    EXPECT_EQ(HAL_AtomicGet(&val), -2);
    HAL_AtomicDec(&val);
    EXPECT_EQ(HAL_AtomicGet(&val), -3);
}

TEST(AtomicSetMask)
{
    HAL_Atomic val;
    HAL_AtomicSet(&val, 0);
    HAL_AtomicSetMask(&val, 0x01);
    EXPECT_EQ(HAL_AtomicGet(&val), 0x01);
    HAL_AtomicSetMask(&val, 0x02);
    EXPECT_EQ(HAL_AtomicGet(&val), 0x03);
    HAL_AtomicSetMask(&val, 0x10);
    EXPECT_EQ(HAL_AtomicGet(&val), 0x13);
}

TEST(AtomicClearMask)
{
    HAL_Atomic val;
    HAL_AtomicSet(&val, 0xffffff);
    HAL_AtomicClearMask(&val, 0x01);
    EXPECT_EQ(HAL_AtomicGet(&val), 0xfffffe);
    HAL_AtomicClearMask(&val, 0x02);
    EXPECT_EQ(HAL_AtomicGet(&val), 0xfffffc);
    HAL_AtomicClearMask(&val, 0x10);
    EXPECT_EQ(HAL_AtomicGet(&val), 0xffffec);
}

TEST(AtomicSwap)
{
    HAL_Atomic val;
    HAL_AtomicSet(&val, 0);
    EXPECT_EQ(HAL_AtomicSwap(&val, 1), 0);
    EXPECT_EQ(HAL_AtomicGet(&val), 1);

    EXPECT_EQ(HAL_AtomicSwap(&val, 0x5a5a5a5a), 1);
    EXPECT_EQ(HAL_AtomicGet(&val), 0x5a5a5a5a);
}

TEST(AtomicCAS)
{
    HAL_Atomic val;
    HAL_AtomicSet(&val, 0);
    EXPECT_EQ(HAL_AtomicCAS(&val, 0, 1), 0);
    EXPECT_EQ(HAL_AtomicGet(&val), 1);

    EXPECT_EQ(HAL_AtomicCAS(&val, 1, 0x5a5a5a5a), 1);
    EXPECT_EQ(HAL_AtomicGet(&val), 0x5a5a5a5a);

    EXPECT_EQ(HAL_AtomicCAS(&val, 1, 0xfafafa), 0x5a5a5a5a);
    EXPECT_NE(HAL_AtomicGet(&val), 0xfafafa);

    EXPECT_EQ(HAL_AtomicCAS(&val, 0x5a5a5a5a, 0xfafafa), 0x5a5a5a5a);
    EXPECT_EQ(HAL_AtomicGet(&val), 0xfafafa);
    
    EXPECT_NE(HAL_AtomicCAS(&val, 0, 1), 0);
    EXPECT_NE(HAL_AtomicGet(&val), 0);
}

TEST_TABLE(Atomic)
{
    TEST_UNIT(AtomicSetAndGet),
    TEST_UNIT(AtomicAdd),
    TEST_UNIT(AtomicSub),
    TEST_UNIT(AtomicInc),
    TEST_UNIT(AtomicDec),
    TEST_UNIT(AtomicSetMask),
    TEST_UNIT(AtomicClearMask),
    TEST_UNIT(AtomicSwap),
    TEST_UNIT(AtomicCAS),
};

TEST_CASE(Atomic);

#endif
