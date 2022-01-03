/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: HAL Multi Core support
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-9      JasonHu           Init
 */

#include <Sched/MultiCore.h>
#define LOG_NAME "Multi Core"
#include <Utils/Log.h>

PUBLIC UArch HAL_CoreGetId(void)
{
    return 0;
}

PUBLIC OS_Error HAL_CoreBootApp(UArch bootCoreId)
{
    LOG_I("boot core is:%d", bootCoreId);
    return OS_ENORES;
}

PUBLIC OS_Error HAL_CoreEnterApp(UArch appCoreId)
{
    return OS_ENORES;
}

INTERFACE struct MultiCoreOps MultiCoreOpsInterface = 
{
    .getId = HAL_CoreGetId,
    .bootApp = HAL_CoreBootApp,
    .enterApp = HAL_CoreEnterApp,
};
