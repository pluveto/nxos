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

#ifdef CONFIG_ENABLE_TEST_UTEST

/* assert */
#define __UTestAssertEx(value, msg) UTestAssert(value, __FILE__, __LINE__, __func__, msg, TRUE)

#define __UTestAssertStringEx(a, b, equ, msg) \
        UTestAssertString((const char*)(a), (const char*)(b), equ, \
        __FILE__, __LINE__, __func__, msg, TRUE)

#define __UTestAssertBufEx(a, b, sz, equ, msg) \
        UTestAssertBuf((const char*)(a), (const char*)(b), (sz), equ, \
        __FILE__, __LINE__, __func__, msg, TRUE)

#define ASSERT_TRUE(value)   __UTestAssertEx(value, "(" #value ") is false")
#define ASSERT_FALSE(value)  __UTestAssertEx(!(value), "(" #value ") is true")

#define ASSERT_EQ(a, b)  __UTestAssertEx((a) == (b), "(" #a ") not equal to (" #b ")")
#define ASSERT_NE(a, b)  __UTestAssertEx((a) != (b),  "(" #a ") equal to (" #b ")")

#define ASSERT_LT(a, b)  __UTestAssertEx((a) < (b), "(" #a ") >= (" #b ")")
#define ASSERT_GT(a, b)  __UTestAssertEx((a) > (b), "(" #a ") <= (" #b ")")
#define ASSERT_LE(a, b)  __UTestAssertEx((a) <= (b), "(" #a ") > (" #b ")")
#define ASSERT_GE(a, b)  __UTestAssertEx((a) >= (b), "(" #a ") < (" #b ")")

#define ASSERT_NULL(value)      __UTestAssertEx((const char *)(value) == NULL, "(" #value ") is not null")
#define ASSERT_NOT_NULL(value)  __UTestAssertEx((const char *)(value) != NULL, "(" #value ") is null")

#define ASSERT_STREQ(a, b)  __UTestAssertStringEx((const char*)(a), (const char*)(b), TRUE, "string not equal")
#define ASSERT_STRNE(a, b)  __UTestAssertStringEx((const char*)(a), (const char*)(b), FALSE, "string equal")

#define ASSERT_BUFEQ(a, b, sz)  __UTestAssertBufEx((const char*)(a), (const char*)(b), (sz), TRUE, "buf not equal")
#define ASSERT_BUFNE(a, b, sz)  __UTestAssertBufEx((const char*)(a), (const char*)(b), (sz), FALSE, "buf equal")

#define ASSERT_IN_RANGE(value, min, max)  __UTestAssertEx(((value >= min) && (value <= max)), "(" #value ") not in range("#min","#max")")
#define ASSERT_NI_RANGE(value, min, max) __UTestAssertEx(!((value >= min) && (value <= max)), "(" #value ") in range("#min","#max")")

#define ASSERT_TRUE_EX(valu, msg)   __UTestAssertEx(value, msg) 
#define ASSERT_FALSE_EX(value, msg)  __UTestAssertEx(!(value), msg)

#define ASSERT_EQ_EX(a, b, msg)  __UTestAssertEx((a) == (b), msg)
#define ASSERT_NE_EX(a, b, msg)  __UTestAssertEx((a) != (b), msg)

#define ASSERT_LT_EX(a, b, msg)  __UTestAssertEx((a) < (b), msg)
#define ASSERT_GT_EX(a, b, msg)  __UTestAssertEx((a) > (b), msg)
#define ASSERT_LE_EX(a, b, msg)  __UTestAssertEx((a) <= (b), msg)
#define ASSERT_GE_EX(a, b, msg)  __UTestAssertEx((a) >= (b), msg)

#define ASSERT_NULL_EX(value, msg)      __UTestAssertEx((const char *)(value) == NULL, msg)
#define ASSERT_NOT_NULL_EX(value, msg)  __UTestAssertEx((const char *)(value) != NULL, msg)

#define ASSERT_STREQ_EX(a, b, msg)  __UTestAssertStringEx((const char*)(a), (const char*)(b), TRUE, msg)
#define ASSERT_STRNE_EX(a, b, msg)  __UTestAssertStringEx((const char*)(a), (const char*)(b), FALSE, msg)

#define ASSERT_BUFEQ_EX(a, b, sz, msg)  __UTestAssertBufEx((const char*)(a), (const char*)(b), (sz), TRUE, msg)
#define ASSERT_BUFNE_EX(a, b, sz, msg)  __UTestAssertBufEx((const char*)(a), (const char*)(b), (sz), FALSE, msg)

#define ASSERT_IN_RANGE_EX(value, min, max, msg)  __UTestAssertEx(((value >= min) && (value <= max)), msg) 
#define ASSERT_NI_RANGE_EX(value, min, max, msg) __UTestAssertEx(!((value >= min) && (value <= max)), msg)

/* expect */
#define __UTestExpectEx(value, msg) UTestAssert(value, __FILE__, __LINE__, __func__, msg, FALSE)

#define __UTestExpectStringEx(a, b, equ, msg) \
        UTestAssertString((const char*)(a), (const char*)(b), equ, \
        __FILE__, __LINE__, __func__, msg, FALSE)

#define __UTestExpectBufEx(a, b, sz, equ, msg) \
        UTestAssertBuf((const char*)(a), (const char*)(b), (sz), equ, \
        __FILE__, __LINE__, __func__, msg, FALSE)

#define EXPECT_TRUE(value)   __UTestExpectEx(value, "(" #value ") is false")
#define EXPECT_FALSE(value)  __UTestExpectEx(!(value), "(" #value ") is true")

#define EXPECT_EQ(a, b)  __UTestExpectEx((a) == (b), "(" #a ") not equal to (" #b ")")
#define EXPECT_NE(a, b)  __UTestExpectEx((a) != (b),  "(" #a ") equal to (" #b ")")

#define EXPECT_LT(a, b)  __UTestExpectEx((a) < (b), "(" #a ") >= (" #b ")")
#define EXPECT_GT(a, b)  __UTestExpectEx((a) > (b), "(" #a ") <= (" #b ")")
#define EXPECT_LE(a, b)  __UTestExpectEx((a) <= (b), "(" #a ") > (" #b ")")
#define EXPECT_GE(a, b)  __UTestExpectEx((a) >= (b), "(" #a ") < (" #b ")")

#define EXPECT_NULL(value)      __UTestExpectEx((const char *)(value) == NULL, "(" #value ") is not null")
#define EXPECT_NOT_NULL(value)  __UTestExpectEx((const char *)(value) != NULL, "(" #value ") is null")

#define EXPECT_STREQ(a, b)  __UTestExpectStringEx((const char*)(a), (const char*)(b), TRUE, "string not equal")
#define EXPECT_STRNE(a, b)  __UTestExpectStringEx((const char*)(a), (const char*)(b), FALSE, "string equal")

#define EXPECT_BUFEQ(a, b, sz)  __UTestExpectBufEx((const char*)(a), (const char*)(b), (sz), TRUE, "buf not equal")
#define EXPECT_BUFNE(a, b, sz)  __UTestExpectBufEx((const char*)(a), (const char*)(b), (sz), FALSE, "buf equal")

#define EXPECT_IN_RANGE(value, min, max)  __UTestExpectEx(((value >= min) && (value <= max)), "(" #value ") not in range("#min","#max")")
#define EXPECT_NI_RANGE(value, min, max) __UTestExpectEx(!((value >= min) && (value <= max)), "(" #value ") in range("#min","#max")")

#define EXPECT_TRUE_EX(valu, msg)   __UTestExpectEx(value, msg) 
#define EXPECT_FALSE_EX(value, msg)  __UTestExpectEx(!(value), msg)

#define EXPECT_EQ_EX(a, b, msg)  __UTestExpectEx((a) == (b), msg)
#define EXPECT_NE_EX(a, b, msg)  __UTestExpectEx((a) != (b), msg)

#define EXPECT_LT_EX(a, b, msg)  __UTestExpectEx((a) < (b), msg)
#define EXPECT_GT_EX(a, b, msg)  __UTestExpectEx((a) > (b), msg)
#define EXPECT_LE_EX(a, b, msg)  __UTestExpectEx((a) <= (b), msg)
#define EXPECT_GE_EX(a, b, msg)  __UTestExpectEx((a) >= (b), msg)

#define EXPECT_NULL_EX(value, msg)      __UTestExpectEx((const char *)(value) == NULL, msg)
#define EXPECT_NOT_NULL_EX(value, msg)  __UTestExpectEx((const char *)(value) != NULL, msg)

#define EXPECT_STREQ_EX(a, b, msg)  __UTestExpectStringEx((const char*)(a), (const char*)(b), TRUE, msg)
#define EXPECT_STRNE_EX(a, b, msg)  __UTestExpectStringEx((const char*)(a), (const char*)(b), FALSE, msg)

#define EXPECT_BUFEQ_EX(a, b, sz, msg)  __UTestExpectBufEx((const char*)(a), (const char*)(b), (sz), TRUE, msg)
#define EXPECT_BUFNE_EX(a, b, sz, msg)  __UTestExpectBufEx((const char*)(a), (const char*)(b), (sz), FALSE, msg)

#define EXPECT_IN_RANGE_EX(value, min, max, msg)  __UTestExpectEx(((value >= min) && (value <= max)), msg) 
#define EXPECT_NI_RANGE_EX(value, min, max, msg) __UTestExpectEx(!((value >= min) && (value <= max)), msg)

/* functions */
PUBLIC void UTestAssert(int value, const char *file, int line, const char *func, const char *msg, Bool dieAction);
PUBLIC void UTestAssertString(const char *a, const char *b, Bool equal, const char *file, int line, const char *func, const char *msg, Bool dieAction);
PUBLIC void UTestAssertBuf(const char *a, const char *b, USize sz, Bool equal, const char *file, int line, const char *func, const char *msg, Bool dieAction);

#endif /* CONFIG_ENABLE_TEST_UTEST */

#endif /* __MODS_UTEST_CONDITION__ */
