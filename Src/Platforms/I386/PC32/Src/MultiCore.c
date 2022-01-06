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
#define NX_LOG_NAME "Multi Core"
#include <Utils/Log.h>

NX_PUBLIC NX_UArch HAL_CoreGetId(void)
{
    return 0;
}

NX_PUBLIC NX_Error HAL_CoreBootApp(NX_UArch bootCoreId)
{
    NX_LOG_I("boot core is:%d", bootCoreId);
    return NX_ENORES;
}

NX_PUBLIC NX_Error HAL_CoreEnterApp(NX_UArch appCoreId)
{
    return NX_ENORES;
}

NX_INTERFACE struct NX_MultiCoreOps NX_MultiCoreOpsInterface = 
{
    .getId = HAL_CoreGetId,
    .bootApp = HAL_CoreBootApp,
    .enterApp = HAL_CoreEnterApp,
};
