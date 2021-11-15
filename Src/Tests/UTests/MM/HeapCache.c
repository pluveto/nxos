/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: heap cache test 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-5      JasonHu           Init
 */

#include <Mods/Test/UTest.h>
#include <MM/HeapCache.h>
#include <Utils/Memory.h>

#ifdef CONFIG_UTEST_HEAP_CACHE

TEST(HeapAllocAndFree)
{
    ASSERT_NULL(HeapAlloc(0));
    ASSERT_EQ(HeapFree(NULL), OS_EINVAL);

    void *p = HeapAlloc(32);
    ASSERT_NOT_NULL(p);
    Zero(p, 32);
    ASSERT_EQ(HeapFree(p), OS_EOK);

    p = HeapAlloc(256);
    ASSERT_NOT_NULL(p);
    Zero(p, 256);
    ASSERT_EQ(HeapFree(p), OS_EOK);

    p = HeapAlloc(128 * 1024);
    ASSERT_NOT_NULL(p);
    Zero(p, 128 * 1024);
    ASSERT_EQ(HeapFree(p), OS_EOK);

    p = HeapAlloc(512 * 1024);
    ASSERT_NOT_NULL(p);
    Zero(p, 512 * 1024);
    ASSERT_EQ(HeapFree(p), OS_EOK);

    p = HeapAlloc(1024*1024*4);
    EXPECT_NOT_NULL(p);
    EXPECT_EQ(HeapFreeSatety(p), OS_EOK);
    EXPECT_NE(HeapFree(p), OS_EOK);
}

TEST_TABLE(HeapCache)
{
    TEST_UNIT(HeapAllocAndFree),
};

TEST_CASE(HeapCache);

#endif
