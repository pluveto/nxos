/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: multi core sched
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-10     JasonHu           Init
 */

#ifndef __SCHED_MULTI_CORE___
#define __SCHED_MULTI_CORE___

#include <XBook.h>

struct MultiCoreOps
{
    Uint (*getId)(void);
    OS_Error (*bootApp)(Uint bootCoreId);
    OS_Error (*enterApp)(Uint appCoreId);
};

INTERFACE IMPORT struct MultiCoreOps MultiCoreOpsInterface; 

#define MultiCoreBootApp MultiCoreOpsInterface.bootApp
#define MultiCoreEnterApp MultiCoreOpsInterface.enterApp
#define MultiCoreGetId MultiCoreOpsInterface.getId

PUBLIC void MultiCoreInit(Uint coreId);
PUBLIC void MultiCoreMain(Uint coreId);
PUBLIC void MultiCoreStage2(Uint appCoreId);

PUBLIC Uint MultiCoreGetBootCore(void);

#endif /* __SCHED_MULTI_CORE___ */
