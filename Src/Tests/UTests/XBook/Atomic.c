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
    Atomic val;
    AtomicSet(&val, 0);
    EXPECT_EQ(AtomicGet(&val), 0);
    
    AtomicSet(&val, 1);
    EXPECT_EQ(AtomicGet(&val), 1);
}

TEST(AtomicAdd)
{
    Atomic val;
    AtomicSet(&val, 0);
    AtomicAdd(&val, 1);
    EXPECT_EQ(AtomicGet(&val), 1);
    AtomicAdd(&val, 3);
    EXPECT_EQ(AtomicGet(&val), 4);
    AtomicAdd(&val, 100);
    EXPECT_EQ(AtomicGet(&val), 104);
}

TEST(AtomicSub)
{
    Atomic val;
    AtomicSet(&val, 0);
    AtomicSub(&val, 1);
    EXPECT_EQ(AtomicGet(&val), -1);
    AtomicSub(&val, 3);
    EXPECT_EQ(AtomicGet(&val), -4);
    AtomicSub(&val, 100);
    EXPECT_EQ(AtomicGet(&val), -104);
}

TEST(AtomicInc)
{
    Atomic val;
    AtomicSet(&val, 0);
    AtomicInc(&val);
    EXPECT_EQ(AtomicGet(&val), 1);
    AtomicInc(&val);
    EXPECT_EQ(AtomicGet(&val), 2);
    AtomicInc(&val);
    EXPECT_EQ(AtomicGet(&val), 3);
}

TEST(AtomicDec)
{
    Atomic val;
    AtomicSet(&val, 0);
    AtomicDec(&val);
    EXPECT_EQ(AtomicGet(&val), -1);
    AtomicDec(&val);
    EXPECT_EQ(AtomicGet(&val), -2);
    AtomicDec(&val);
    EXPECT_EQ(AtomicGet(&val), -3);
}

TEST(AtomicSetMask)
{
    Atomic val;
    AtomicSet(&val, 0);
    AtomicSetMask(&val, 0x01);
    EXPECT_EQ(AtomicGet(&val), 0x01);
    AtomicSetMask(&val, 0x02);
    EXPECT_EQ(AtomicGet(&val), 0x03);
    AtomicSetMask(&val, 0x10);
    EXPECT_EQ(AtomicGet(&val), 0x13);
}

TEST(AtomicClearMask)
{
    Atomic val;
    AtomicSet(&val, 0xffffff);
    AtomicClearMask(&val, 0x01);
    EXPECT_EQ(AtomicGet(&val), 0xfffffe);
    AtomicClearMask(&val, 0x02);
    EXPECT_EQ(AtomicGet(&val), 0xfffffc);
    AtomicClearMask(&val, 0x10);
    EXPECT_EQ(AtomicGet(&val), 0xffffec);
}

TEST(AtomicSwap)
{
    Atomic val;
    AtomicSet(&val, 0);
    EXPECT_EQ(AtomicSwap(&val, 1), 0);
    EXPECT_EQ(AtomicGet(&val), 1);

    EXPECT_EQ(AtomicSwap(&val, 0x5a5a5a5a), 1);
    EXPECT_EQ(AtomicGet(&val), 0x5a5a5a5a);
}

TEST(AtomicCAS)
{
    Atomic val;
    AtomicSet(&val, 0);
    EXPECT_EQ(AtomicCAS(&val, 0, 1), 0);
    EXPECT_EQ(AtomicGet(&val), 1);

    EXPECT_EQ(AtomicCAS(&val, 1, 0x5a5a5a5a), 1);
    EXPECT_EQ(AtomicGet(&val), 0x5a5a5a5a);

    EXPECT_EQ(AtomicCAS(&val, 1, 0xfafafa), 0x5a5a5a5a);
    EXPECT_NE(AtomicGet(&val), 0xfafafa);

    EXPECT_EQ(AtomicCAS(&val, 0x5a5a5a5a, 0xfafafa), 0x5a5a5a5a);
    EXPECT_EQ(AtomicGet(&val), 0xfafafa);
    
    EXPECT_NE(AtomicCAS(&val, 0, 1), 0);
    EXPECT_NE(AtomicGet(&val), 0);
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
