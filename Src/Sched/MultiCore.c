/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: mutil core sched
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-10     JasonHu           Init
 */

#include <Sched/MultiCore.h>
#include <Sched/Thread.h>
#define LOG_NAME "Core"
#include <Utils/Log.h>

/* init as zero, avoid cleared by clear bss action */
PRIVATE VOLATILE Uint BootCoreId = 0;

PUBLIC void MultiCoreInit(Uint coreId)
{
    /* recored boot core */
    BootCoreId = coreId;
}

PUBLIC Uint MultiCoreGetBootCore(void)
{
    return BootCoreId;
}

PUBLIC void MultiCoreMain(Uint coreId)
{
    if (MultiCoreBootApp(coreId) != OS_EOK)
    {
        LOG_W("Boot multi core failed!");
    }
    else
    {
        LOG_I("Boot multi core success!");
    }
}

PUBLIC void MultiCoreStage2(Uint appCoreId)
{
    LOG_I("core %d stage2", appCoreId);
    OS_Error err;
    err = MultiCoreEnterApp(appCoreId);
    if (err != OS_EOK)
    {
        LOG_E("app core: %d setup failed!", appCoreId);
    }
    else
    {
        LOG_I("app core: %d setup success!", appCoreId);
    }
}
