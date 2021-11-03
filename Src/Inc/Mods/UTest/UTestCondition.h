/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Unit test condition
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-2      JasonHu           Init
 */

#ifndef __MODS_UTEST_CONDITION__
#define __MODS_UTEST_CONDITION__

#include <XBook.h>

#define __UTestAssert(value, msg) UTestAssert(value, __FILE__, __LINE__, __func__, msg)

#define ASSERT_TRUE(value)   __UTestAssert(value, "[" #value "] NOT TRUE") 
#define ASSERT_FALSE(value)  __UTestAssert(!(value), "[" #value "] NOT FALSE")

#define ASSERT_EQ(a, b)  __UTestAssert((a) == (b), "[" #a "] NOT EQUAL [" #b "]")
#define ASSERT_NE(a, b)  __UTestAssert((a) != (b), "[" #a "] EQUAL [" #b "]")

#define ASSERT_LT(a, b)  __UTestAssert((a) < (b), "[" #a "] >= [" #b "]")
#define ASSERT_GT(a, b)  __UTestAssert((a) > (b), "[" #a "] <= [" #b "]")
#define ASSERT_LE(a, b)  __UTestAssert((a) <= (b), "[" #a "] > [" #b "]")
#define ASSERT_GE(a, b)  __UTestAssert((a) >= (b), "[" #a "] < [" #b "]")

#define ASSERT_NULL(value)      __UTestAssert((const char *)(value) == NULL, "[" #value "] NOT NULL")
#define ASSERT_NOT_NULL(value)  __UTestAssert((const char *)(value) != NULL, "[" #value "] NULL")

#define ASSERT_STREQ(a, b)  UTestAssertString((const char*)(a), (const char*)(b), TRUE, __FILE__, __LINE__, __func__, "string not equal")
#define ASSERT_STRNE(a, b)  UTestAssertString((const char*)(a), (const char*)(b), FALSE, __FILE__, __LINE__, __func__, "string equal")

#define ASSERT_BUFEQ(a, b, sz)  UTestAssertBuf((const char*)(a), (const char*)(b), (sz), TRUE, __FILE__, __LINE__, __func__, "buf not equal")
#define ASSERT_BUFNE(a, b, sz)  UTestAssertBuf((const char*)(a), (const char*)(b), (sz), FALSE, __FILE__, __LINE__, __func__, "buf equal")

#define ASSERT_IN_RANGE(value, min, max)  __UTestAssert(((value >= min) && (value <= max)), "[" #value "] NOT IN RANGE ["#min","#max"]") 
#define ASSERT_NI_RANGE(value, min, max) __UTestAssert(!((value >= min) && (value <= max)), "[" #value "] IN RANGE ["#min","#max"]")

PUBLIC void UTestAssert(int value, const char *file, int line, const char *func, const char *msg);
PUBLIC void UTestAssertString(const char *a, const char *b, Bool equal, const char *file, int line, const char *func, const char *msg);
PUBLIC void UTestAssertBuf(const char *a, const char *b, Size sz, Bool equal, const char *file, int line, const char *func, const char *msg);

#endif /* __MODS_UTEST_CONDITION__ */
