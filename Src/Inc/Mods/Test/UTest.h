/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Unit test
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-2      JasonHu           Init
 */

#ifndef __MODS_UTEST__
#define __MODS_UTEST__

#include <XBook/InitCall.h>
#include <Utils/Log.h>

#ifdef CONFIG_ENABLE_TEST_UTEST

#include <Mods/Test/UTestCondition.h>

struct UTest
{
    char *testName;
    void (*func)(void);
    void (*setup)(void);
    void (*clean)(void);
};
typedef struct UTest UTest;

struct UTestCase
{
    char *caseName;
    OS_Error (*setup)(void);
    OS_Error (*clean)(void);
    UTest *unitTable;
    Size unitCount;
};
typedef struct UTestCase UTestCase;

struct UTestSum
{
    Bool hasError;
    U32 passedNum;
    U32 failedNum;
};
typedef struct UTestSum UTestSum;

#define TEST(name) \
    PRIVATE void Utest##name(void)

#define TEST_SETUP(name) \
    PRIVATE void Utest##name##Setup(void)

#define TEST_CLEAN(name) \
    PRIVATE void Utest##name##Clean(void)

#define TEST_TABLE(name) \
    USED PRIVATE UTest __UTestCase##name##Table[] =  

#define TEST_UNIT(name) \
    { \
        #name, \
        Utest##name, \
        NULL, \
        NULL \
    }

#define TEST_UNIT_SETUP_CLEAN(name) \
    { \
        #name, \
        Utest##name, \
        Utest##name##Setup, \
        Utest##name##Clean \
    }

#define TEST_UNIT_SETUP(name) \
    { \
        #name, \
        Utest##name, \
        Utest##name##Setup, \
        NULL \
    }

#define TEST_UNIT_CLEAN(name) \
    { \
        #name, \
        Utest##name, \
        NULL, \
        Utest##name##Clean \
    }

#define TEST_CASE_SETUP(name)  OS_Error __UTestCase##name##Setup(void)
#define TEST_CASE_CLEAN(name)  OS_Error __UTestCase##name##Clean(void)

#define TEST_CASE(name) \
    TEST_CASE_SETUP(name) WEAK_SYM;\
    TEST_CASE_CLEAN(name) WEAK_SYM;\
    USED PRIVATE const UTestCase __utestTestcase \
    SECTION("UTestCaseTable") = \
    { \
        .caseName = #name, \
        .setup = __UTestCase##name##Setup, \
        .clean = __UTestCase##name##Clean, \
        .unitTable = __UTestCase##name##Table, \
        .unitCount = ARRAY_SIZE(__UTestCase##name##Table) \
    }

PUBLIC void UTestInit(void);
/* invoke utest */
PUBLIC void UTestInvoke(void);

#endif /* CONFIG_ENABLE_TEST_UTEST */

#endif  /* __MODS_UTEST__ */
