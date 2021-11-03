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

#include <Mods/UTest/UTest.h>

TEST(Test)
{
    ASSERT_TRUE(1);
    ASSERT_TRUE(TRUE);
    ASSERT_FALSE(0);
    ASSERT_FALSE(FALSE);
}

TEST(Test1)
{
    ASSERT_EQ(1, 1);
    ASSERT_NE(0, 1);
    
    ASSERT_LT(3, 5);
    ASSERT_LE(3, 3);
    ASSERT_LE(3, 4);
    ASSERT_GT(4, 3);
    ASSERT_GE(3, 3);
    
    char *p = NULL;
    ASSERT_NULL(p);
    char *q = (char *) 0x1000;
    ASSERT_NOT_NULL(q);
}

TEST(Test2)
{
    ASSERT_IN_RANGE(1, 0, 5);
    ASSERT_IN_RANGE(4, 0, 5);
    ASSERT_IN_RANGE(1, 1, 5);
    ASSERT_IN_RANGE(5, 1, 5);

    ASSERT_NI_RANGE(1, 2, 5);
    ASSERT_NI_RANGE(0, 1, 5);
    ASSERT_NI_RANGE(6, 1, 5);
    ASSERT_NI_RANGE(5, 1, 4);
}

TEST(Test3)
{
    ASSERT_STREQ("hello", "hello");
    char *s = "world!";
    ASSERT_STREQ(s, "world!");

    char buf0[32] = {0x10, 0x20, 0x30,};
    char buf1[32] = {0x10, 0x20, 0x30,};
    ASSERT_BUFEQ(buf0, buf1, 32);

    ASSERT_STRNE("hello", "world");
    s = "hello!";
    ASSERT_STRNE(s, "world!");

    char buf2[32] = {0x40, 0x50, 0x60,};
    ASSERT_BUFNE(buf1, buf2, 32);
}

TEST(Test4)
{
    ASSERT_TRUE(0);
    ASSERT_FALSE(1);
}

TEST_SETUP(Test)
{
    ASSERT_TRUE(1);
}

TEST_CLEAN(Test)
{
    ASSERT_FALSE(0);
}

TEST_TABLE(UTest)
{
    TEST_UNIT(Test),
    TEST_UNIT_SETUP_CLEAN(Test),
    TEST_UNIT(Test1),
    TEST_UNIT(Test2),
    TEST_UNIT(Test3),
    TEST_UNIT(Test4),
};

TEST_CASE_SETUP(UTest)
{
    ASSERT_TRUE(1);
    return OS_EOK;
}

TEST_CASE_CLEAN(UTest)
{
    ASSERT_FALSE(0);
    return OS_EOK;
}

TEST_CASE(UTest);
