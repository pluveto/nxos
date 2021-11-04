/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: string test 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-2      JasonHu           Init
 */

#include <Utils/String.h>
#include <Mods/UTest/UTest.h>

TEST(StrCmp)
{
    const char *str = "abc";
    EXPECT_EQ(StrCmp(NULL, NULL), 0);
    EXPECT_EQ(StrCmp(str, NULL), 0);
    EXPECT_EQ(StrCmp(NULL, str), 0);
    
    EXPECT_EQ(StrCmp(str, "abc"), 0);
    EXPECT_NE(StrCmp(str, "cba"), 0);
    
    EXPECT_EQ(StrCmp(str, "abd"), -1);
    EXPECT_EQ(StrCmp(str, "abb"), 1);
}

TEST(StrCopy)
{
    const char *str = "hello!";
    EXPECT_NULL(StrCopy(NULL, NULL));
    EXPECT_NULL(StrCopy(str, NULL));
    EXPECT_NULL(StrCopy(NULL, str));
    char buf[10] = {0};
    EXPECT_NOT_NULL(StrCopy(buf, str));
    EXPECT_STREQ(buf, str);
}

TEST(StrLen)
{
    const char *str = "hello!";
    EXPECT_EQ(StrLen(NULL), 0);
    EXPECT_NE(StrLen(str), 0);
    EXPECT_EQ(StrLen(str), 6);
}

TEST_TABLE(String)
{
    TEST_UNIT(StrCmp),
    TEST_UNIT(StrCopy),
    TEST_UNIT(StrLen),
};

TEST_CASE(String);
