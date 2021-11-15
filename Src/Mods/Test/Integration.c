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
#include <Utils/Debug.h>
#include <Sched/Thread.h>
#include <XBook/InitCall.h>

PRIVATE Integration *integrationTable = NULL;
PRIVATE Size integrationCount;

IMPORT const Addr __IntegrationTableStart;
IMPORT const Addr __IntegrationTableEnd;

PRIVATE void IntegrationInvoke(void)
{
    integrationTable = (Integration *)&__IntegrationTableStart;
    integrationCount = (Integration *) &__IntegrationTableEnd - integrationTable;
    LOG_I("[==========] Total integrations: %d", integrationCount);
    int integrationIndex;
    OS_Error err;
    Size passedTests = 0; 
    for (integrationIndex = 0; integrationIndex < integrationCount; integrationIndex++)
    {
        LOG_I("[==========] [ integration ] Running (%d/%d) test (%s).", integrationIndex + 1, integrationCount, integrationTable->integrationName);
        err = integrationTable->func();
        if (err == OS_EOK)
        {
            passedTests++;
            LOG_I("[==========] [ integration ] (%d/%d) test ran with state %s .",
                integrationIndex + 1, integrationCount, "success");
        }
        else
        {
            LOG_E("[==========] [ integration ] (%d/%d) test ran with state %s .",
                integrationIndex + 1, integrationCount, "failed");
            break;
        }
        integrationTable++;
    }
    LOG_I("[  FINAL   ] %d integration test finshed. %d/%d are passed. %d/%d are failed.",
        integrationCount, passedTests, integrationCount, integrationCount - passedTests, integrationCount);
}

PRIVATE void IntegrationEntry(void *arg)
{
    /* call integration */
    IntegrationInvoke();
}

PUBLIC void IntegrationInit(void)
{
    Printf("IntegrationInit");
    Thread *thread = ThreadCreate("Integration", IntegrationEntry, NULL);
    ASSERT(thread != NULL);
    ASSERT(ThreadRun(thread) == OS_EOK);
}

INIT_TEST(IntegrationInit);

#endif
