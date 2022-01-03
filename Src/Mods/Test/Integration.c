/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Integration test
 *           
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-15     JasonHu           Init
 */

#include <Mods/Test/Integration.h>

#ifdef CONFIG_ENABLE_TEST_INTEGRATION

#include <Utils/String.h>
#include <Utils/Memory.h>
#include <XBook/Debug.h>
#include <Sched/Thread.h>
#include <XBook/InitCall.h>

PRIVATE Integration *IntegrationTable = NULL;
PRIVATE USize IntegrationCount;

IMPORT const Addr __IntegrationTableStart;
IMPORT const Addr __IntegrationTableEnd;

PRIVATE void IntegrationInvoke(void)
{
    IntegrationTable = (Integration *)&__IntegrationTableStart;
    IntegrationCount = (Integration *) &__IntegrationTableEnd - IntegrationTable;
    LOG_I("[==========] Total integrations: %d", IntegrationCount);
    int integrationIndex;
    OS_Error err;
    USize passedTests = 0; 
    for (integrationIndex = 0; integrationIndex < IntegrationCount; integrationIndex++)
    {
        LOG_I("[==========] [ integration ] Running (%d/%d) test (%s).", integrationIndex + 1, IntegrationCount, IntegrationTable->integrationName);
        err = IntegrationTable->func();
        if (err == OS_EOK)
        {
            passedTests++;
            LOG_I("[==========] [ integration ] (%d/%d) test ran with state %s .",
                integrationIndex + 1, IntegrationCount, "success");
        }
        else
        {
            LOG_E("[==========] [ integration ] (%d/%d) test ran with state %s .",
                integrationIndex + 1, IntegrationCount, "failed");
            break;
        }
        IntegrationTable++;
    }
    LOG_I("[  FINAL   ] %d integration test finshed. %d/%d are passed. %d/%d are failed.",
        IntegrationCount, passedTests, IntegrationCount, IntegrationCount - passedTests, IntegrationCount);
}

PRIVATE void IntegrationEntry(void *arg)
{
    /* call integration */
    IntegrationInvoke();
}

PUBLIC void IntegrationInit(void)
{
    Thread *thread = ThreadCreate("Integration", IntegrationEntry, NULL);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);
}

INIT_TEST(IntegrationInit);

#endif
