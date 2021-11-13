/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Unit test
 *           refernce rt-thread
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-2      JasonHu           Init
 */

#include <Mods/UTest/UTest.h>
#include <Utils/String.h>
#include <Utils/Memory.h>
#include <Utils/Debug.h>

#include <Sched/Thread.h>

PRIVATE UTestCase *testCaseTable = NULL;
PRIVATE Size testCaseCount;
PRIVATE UTestSum localUtestSum = {FALSE, 0, 0};
PRIVATE UTestSum utestSum = {FALSE, 0, 0};
PRIVATE UTestSum utestCaseSum = {FALSE, 0, 0};

IMPORT const Addr __UTestCaseTableStart;
IMPORT const Addr __UTestCaseTableEnd;

PRIVATE void UTestInvoke(void)
{
    utestCaseSum.hasError = FALSE;
    utestCaseSum.passedNum = 0;
    utestCaseSum.failedNum = 0;

    testCaseTable = (UTestCase *)&__UTestCaseTableStart;
    testCaseCount = (UTestCase *) &__UTestCaseTableEnd - testCaseTable;
    LOG_I("[==========] Total test case: %d", testCaseCount);
    int testCaseIndex;
    int testIndex;
    for (testCaseIndex = 0; testCaseIndex < testCaseCount; testCaseIndex++)
    {
        LOG_I("[==========] [ testcase ] Running %d tests from test case (%s).", testCaseTable->unitCount, testCaseTable->caseName);
        if (testCaseTable->setup != NULL)
        {
            LOG_I("[----------] [ testcase ] Global test (%s) set-up.", testCaseTable->caseName);
            if (testCaseTable->setup() != OS_EOK)
            {
                LOG_E("[  FAILED  ] [ testcase ] Global test (%s) set-up.", testCaseTable->caseName);
                utestCaseSum.failedNum++;
                goto __TestCaseContinue;
            }
        }

        if (testCaseTable->unitTable != NULL)
        {
            utestSum.hasError = FALSE;
            utestSum.passedNum = 0;
            utestSum.failedNum = 0;
            for (testIndex = 0; testIndex < testCaseTable->unitCount; testIndex++)
            {
                UTest *utest = (UTest *)&testCaseTable->unitTable[testIndex];

                if (utest->setup != NULL)
                {
                    LOG_I("[----------] [   test   ] Local test (%s.%s) set-up.", testCaseTable->caseName, utest->testName);
                    utest->setup();
                }
                if (utest->func != NULL)
                {
                    LOG_I("[ RUN      ] [   test   ] %s.%s", testCaseTable->caseName, utest->testName);
                    localUtestSum.hasError = FALSE;
                    localUtestSum.passedNum = 0;
                    localUtestSum.failedNum = 0;
                    utest->func();
                    if (localUtestSum.failedNum == 0)
                    {
                        LOG_I("[  PASSED  ] [   test   ] %s.%s", testCaseTable->caseName, utest->testName);
                        utestSum.passedNum++;
                    }
                    else
                    {
                        LOG_E("[  FAILED  ] [   test   ] %s.%s", testCaseTable->caseName, utest->testName);
                        utestSum.failedNum++;
                    }
                    LOG_I("[   SUM    ] [   test   ] test finshed. %d are passed. %d are failed.", 
                        localUtestSum.passedNum, localUtestSum.failedNum);
                }
                else
                {            
                    LOG_E("[  FAILED  ] [   test   ] %s.%s", testCaseTable->caseName, utest->testName);
                }

                if (utest->clean != NULL)
                {
                    LOG_I("[----------] [   test   ] Local test (%s.%s) tear-down.", testCaseTable->caseName, utest->testName);
                    utest->clean();
                }
            }
            if (utestSum.failedNum == 0)
            {
                utestCaseSum.passedNum++;
            }
            else
            {
                utestCaseSum.failedNum++;
            }
            LOG_I("[   SUM    ] [ testcase ] %d tests finshed. %d/%d are passed. %d/%d are failed.", 
                testCaseTable->unitCount, utestSum.passedNum, testCaseTable->unitCount, 
                utestSum.failedNum, testCaseTable->unitCount);

        }
        else
        {
            LOG_E("[  FAILED  ] [ testcase ] %s", testCaseTable->caseName);
        }

        if (testCaseTable->clean != NULL)
        {
            LOG_I("[----------] [ testcase ] Global test (%s) tear-down.", testCaseTable->caseName);
            if (testCaseTable->clean() != OS_EOK)
            {
                LOG_E("[  FAILED  ] [ testcase ] Global test (%s) tear-down.", testCaseTable->caseName);
                utestCaseSum.failedNum++;
                goto __TestCaseContinue;
            }
        }
__TestCaseContinue:
        LOG_I("[==========] [ testcase ] %d tests from test case (%s) ran.",
            testIndex > 0 ? testIndex + 1 : 0, testCaseTable->caseName);
        testCaseTable++;
    }
    LOG_I("[   SUM    ] %d test cases finshed. %d/%d are passed. %d/%d are failed.",
        testCaseCount, utestCaseSum.passedNum, testCaseCount, utestCaseSum.failedNum, testCaseCount);

}

PUBLIC void UTestAssert(int value, const char *file, int line, const char *func, const char *msg, Bool dieAction)
{
    if (value)
    {
        localUtestSum.hasError = FALSE;
        localUtestSum.passedNum++;
        LOG_I("[       OK ] [   point  ] %s:%d", func, line);
    }
    else
    {
        localUtestSum.hasError = TRUE;
        localUtestSum.failedNum++;
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

PUBLIC void UTestAssertBuf(const char *a, const char *b, Size sz, Bool equal,
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
#ifdef CONFIG_UTEST_INVOKE
    Thread *thread = ThreadCreate("UTest", UTestEntry, NULL);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);
#endif /* CONFIG_UTEST_INVOKE */
}
