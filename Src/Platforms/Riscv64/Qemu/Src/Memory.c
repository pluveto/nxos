/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Page init 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-28     JasonHu           Init
 */

#include <Platform.h>

#include <Utils/Memory.h>

#define LOG_LEVEL LOG_INFO
#define LOG_NAME "Page"
#include <Utils/Log.h>

IMPORT Addr __OS_BssStart;
IMPORT Addr __OS_BssEnd;

PUBLIC void ClearBSS(void)
{
    Zero(&__OS_BssStart, &__OS_BssEnd - &__OS_BssStart);
}
