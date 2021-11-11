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

PRIVATE UTestCase *testCaseTable = NULL;
PRIVATE Size testCaseCount;
PRIVATE UTestSum localUtestSum = {FALSE, 0, 0};
PRIVATE UTestSum utestSum = {FALSE, 0, 0};
PRIVATE UTestSum utestCaseSum = {FALSE, 0, 0};

IMPORT const Addr __UTestCaseTableStart;
IMPORT const Addr __UTestCaseTableEnd;

PUBLIC void UTestInit(void)
{
    utestCaseSum.hasError = FALSE;
    utestCaseSum.passedNum = 0;
    utestCaseSum.failedNum = 0;

    testCaseTable = (UTestCase *)&__UTestCaseTableStart;
    testCaseCount = (UTestCase *) &__UTestCaseTableEnd - testCaseTable;
    LOG_I("[==========] Total test case: " $d(testCaseCount));
    int testCaseIndex;
    int testIndex;
    for (testCaseIndex = 0; testCaseIndex < testCaseCount; testCaseIndex++)
    {
        LOG_I("[==========] [ testcase ] Running " $d(testCaseTable->unitCount) " tests from test case (" $s(testCaseTable->caseName) ").");
        if (testCaseTable->setup != NULL)
        {
            LOG_I("[----------] [ testcase ] Global test (" $(testCaseTable->caseName) ") set-up.");
            if (testCaseTable->setup() != OS_EOK)
            {
                LOG_E("[  FAILED  ] [ testcase ] Global test (" $(testCaseTable->caseName) ") set-up.");
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
                    LOG_I("[----------] [   test   ] Local test (" $(testCaseTable->caseName) "." $(utest->testName) ") set-up.");
                    utest->setup();
                }
                if (utest->func != NULL)
                {
                    LOG_I("[ RUN      ] [   test   ] " $(testCaseTable->caseName) "." $(utest->testName));
                    localUtestSum.hasError = FALSE;
                    localUtestSum.passedNum = 0;
                    localUtestSum.failedNum = 0;
                    utest->func();
                    if (localUtestSum.failedNum == 0)
                    {
                        LOG_I("[  PASSED  ] [   test   ] " $(testCaseTable->caseName) "." $(utest->testName));
                        utestSum.passedNum++;
                    }
                    else
                    {
                        LOG_E("[  FAILED  ] [   test   ] " $(testCaseTable->caseName) "." $(utest->testName));
                        utestSum.failedNum++;
                    }
                    LOG_I("[   SUM    ] [   test   ] test finshed. " \
                        $d(localUtestSum.passedNum) " are passed. " \
                        $d(localUtestSum.failedNum) " are failed.");
                }
                else
                {            
                    LOG_E("[  FAILED  ] [   test   ] " $(testCaseTable->caseName) "." $(utest->testName));
                }

                if (utest->clean != NULL)
                {
                    LOG_I("[----------] [   test   ] Local test (" $(testCaseTable->caseName) "." $(utest->testName) ") tear-down.");
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
            LOG_I("[   SUM    ] [ testcase ] " $d(testCaseTable->unitCount) " tests finshed. " \
                $d(utestSum.passedNum) "/" $d(testCaseTable->unitCount) " are passed. " \
                $d(utestSum.failedNum) "/" $d(testCaseTable->unitCount) " are failed.");

        }
        else
        {
            LOG_E("[  FAILED  ] [ testcase ] " $(testCaseTable->caseName));
        }

        if (testCaseTable->clean != NULL)
        {
            LOG_I("[----------] [ testcase ] Global test (" $(testCaseTable->caseName) ") tear-down.");
            if (testCaseTable->clean() != OS_EOK)
            {
                LOG_E("[  FAILED  ] [ testcase ] Global test (" $(testCaseTable->caseName) ") tear-down.");
                utestCaseSum.failedNum++;
                goto __TestCaseContinue;
            }
        }
__TestCaseContinue:
        LOG_I("[==========] [ testcase ] " $d(testIndex > 0 ? testIndex + 1 : 0) " tests from test case (" $s(testCaseTable->caseName) ") ran.");
        testCaseTable++;
    }
    LOG_I("[   SUM    ] " $d(testCaseCount) " test cases finshed. " \
        $d(utestCaseSum.passedNum) "/" $d(testCaseCount) " are passed. " \
        $d(utestCaseSum.failedNum) "/" $d(testCaseCount) " are failed.");

}

PUBLIC void UTestAssert(int value, const char *file, int line, const char *func, const char *msg, Bool dieAction)
{
    if (value)
    {
        localUtestSum.hasError = FALSE;
        localUtestSum.passedNum++;
        LOG_I("[       OK ] [   point  ] " $(func) ":" $d(line));
    }
    else
    {
        localUtestSum.hasError = TRUE;
        localUtestSum.failedNum++;
        LOG_E("[  FAILED  ] [   point  ] Failure at:" $(file) " Line:" $d(line) " Message:" $(msg));
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
