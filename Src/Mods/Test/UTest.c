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

#include <Mods/Test/UTest.h>

#ifdef CONFIG_ENABLE_TEST_UTEST

#include <Utils/String.h>
#include <Utils/Memory.h>
#include <Utils/Debug.h>
#include <Sched/Thread.h>
#include <XBook/InitCall.h>

PRIVATE UTestCase *TestCaseTable = NULL;
PRIVATE USize TestCaseCount;
PRIVATE UTestSum LocalUtestSum = {FALSE, 0, 0};
PRIVATE UTestSum UtestSum = {FALSE, 0, 0};
PRIVATE UTestSum UtestCaseSum = {FALSE, 0, 0};

IMPORT const Addr __UTestCaseTableStart;
IMPORT const Addr __UTestCaseTableEnd;

PUBLIC void UTestInvoke(void)
{
    UtestCaseSum.hasError = FALSE;
    UtestCaseSum.passedNum = 0;
    UtestCaseSum.failedNum = 0;

    TestCaseTable = (UTestCase *)&__UTestCaseTableStart;
    TestCaseCount = (UTestCase *) &__UTestCaseTableEnd - TestCaseTable;
    LOG_I("[==========] Total test case: %d", TestCaseCount);
    int testCaseIndex = 0;
    int testIndex = 0;
    for (testCaseIndex = 0; testCaseIndex < TestCaseCount; testCaseIndex++)
    {
        LOG_I("[==========] [ testcase ] Running %d tests from test case (%s).", TestCaseTable->unitCount, TestCaseTable->caseName);
        if (TestCaseTable->setup != NULL)
        {
            LOG_I("[----------] [ testcase ] Global test (%s) set-up.", TestCaseTable->caseName);
            if (TestCaseTable->setup() != OS_EOK)
            {
                LOG_E("[  FAILED  ] [ testcase ] Global test (%s) set-up.", TestCaseTable->caseName);
                UtestCaseSum.failedNum++;
                goto __TestCaseContinue;
            }
        }

        if (TestCaseTable->unitTable != NULL)
        {
            UtestSum.hasError = FALSE;
            UtestSum.passedNum = 0;
            UtestSum.failedNum = 0;
            for (testIndex = 0; testIndex < TestCaseTable->unitCount; testIndex++)
            {
                UTest *utest = (UTest *)&TestCaseTable->unitTable[testIndex];

                if (utest->setup != NULL)
                {
                    LOG_I("[----------] [   test   ] Local test (%s.%s) set-up.", TestCaseTable->caseName, utest->testName);
                    utest->setup();
                }
                if (utest->func != NULL)
                {
                    LOG_I("[ RUN      ] [   test   ] %s.%s", TestCaseTable->caseName, utest->testName);
                    LocalUtestSum.hasError = FALSE;
                    LocalUtestSum.passedNum = 0;
                    LocalUtestSum.failedNum = 0;
                    utest->func();
                    if (LocalUtestSum.failedNum == 0)
                    {
                        LOG_I("[  PASSED  ] [   test   ] %s.%s", TestCaseTable->caseName, utest->testName);
                        UtestSum.passedNum++;
                    }
                    else
                    {
                        LOG_E("[  FAILED  ] [   test   ] %s.%s", TestCaseTable->caseName, utest->testName);
                        UtestSum.failedNum++;
                    }
                    LOG_I("[   SUM    ] [   test   ] test finshed. %d are passed. %d are failed.", 
                        LocalUtestSum.passedNum, LocalUtestSum.failedNum);
                }
                else
                {            
                    LOG_E("[  FAILED  ] [   test   ] %s.%s", TestCaseTable->caseName, utest->testName);
                }

                if (utest->clean != NULL)
                {
                    LOG_I("[----------] [   test   ] Local test (%s.%s) tear-down.", TestCaseTable->caseName, utest->testName);
                    utest->clean();
                }
            }
            if (UtestSum.failedNum == 0)
            {
                UtestCaseSum.passedNum++;
            }
            else
            {
                UtestCaseSum.failedNum++;
            }
            LOG_I("[   SUM    ] [ testcase ] %d tests finshed. %d/%d are passed. %d/%d are failed.", 
                TestCaseTable->unitCount, UtestSum.passedNum, TestCaseTable->unitCount, 
                UtestSum.failedNum, TestCaseTable->unitCount);

        }
        else
        {
            LOG_E("[  FAILED  ] [ testcase ] %s", TestCaseTable->caseName);
        }

        if (TestCaseTable->clean != NULL)
        {
            LOG_I("[----------] [ testcase ] Global test (%s) tear-down.", TestCaseTable->caseName);
            if (TestCaseTable->clean() != OS_EOK)
            {
                LOG_E("[  FAILED  ] [ testcase ] Global test (%s) tear-down.", TestCaseTable->caseName);
                UtestCaseSum.failedNum++;
                goto __TestCaseContinue;
            }
        }
__TestCaseContinue:
        LOG_I("[==========] [ testcase ] %d tests from test case (%s) ran.",
            testIndex > 0 ? testIndex + 1 : 0, TestCaseTable->caseName);
        TestCaseTable++;
    }
    LOG_I("[  FINAL   ] %d test cases finshed. %d/%d are passed. %d/%d are failed.",
        TestCaseCount, UtestCaseSum.passedNum, TestCaseCount, UtestCaseSum.failedNum, TestCaseCount);

}

PUBLIC void UTestAssert(int value, const char *file, int line, const char *func, const char *msg, Bool dieAction)
{
    if (value)
    {
        LocalUtestSum.hasError = FALSE;
        LocalUtestSum.passedNum++;
        LOG_I("[       OK ] [   point  ] %s:%d", func, line);
    }
    else
    {
        LocalUtestSum.hasError = TRUE;
        LocalUtestSum.failedNum++;
        LOG_E("[  FAILED  ] [   point  ] Failure at:%s Line:%d Message:%s", file, line, msg);
        if (dieAction)
        {
            /* TODO: exit thread? */
            PANIC("Assert!");
        }
    }
}

PUBLIC void UTestAssertString(const char *a, const char *b, Bool equal, 
    const char *file, int line, const char *func, const char *msg, Bool dieAction)
{
    if (a == NULL || b == NULL)
    {
        UTestAssert(0, file, line, func, msg, dieAction);
    }

    if (equal)
    {
        if (StrCmp(a, b) == 0)
        {
            UTestAssert(1, file, line, func, msg, dieAction);
        }
        else
        {
            UTestAssert(0, file, line, func, msg, dieAction);
        }
    }
    else
    {
        if (StrCmp(a, b) == 0)
        {
            UTestAssert(0, file, line, func, msg, dieAction);
        }
        else
        {
            UTestAssert(1, file, line, func, msg, dieAction);
        }
    }
}

PUBLIC void UTestAssertBuf(const char *a, const char *b, USize sz, Bool equal,
    const char *file, int line, const char *func, const char *msg, Bool dieAction)
{
    if (a == NULL || b == NULL)
    {
        UTestAssert(0, file, line, func, msg, dieAction);
    }

    if (equal)
    {
        if (CompareN(a, b, sz) == 0)
        {
            UTestAssert(1, file, line, func, msg, dieAction);
        }
        else
        {
            UTestAssert(0, file, line, func, msg, dieAction);
        }
    }
    else
    {
        if (CompareN(a, b, sz) == 0)
        {
            UTestAssert(0, file, line, func, msg, dieAction);
        }
        else
        {
            UTestAssert(1, file, line, func, msg, dieAction);
        }
    }
}

PRIVATE void UTestEntry(void *arg)
{
    /* call utest */
    UTestInvoke();
}

PUBLIC void UTestInit(void)
{
    Thread *thread = ThreadCreate("UTest", UTestEntry, NULL);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);
}

INIT_TEST(UTestInit);

#endif
