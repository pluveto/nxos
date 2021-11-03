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
    LOG_D("StrCmp TEST");
}

TEST_TABLE(String)
{
    TEST_UNIT(StrCmp),
};

TEST_CASE(String);
