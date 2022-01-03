/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Console output
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

#include <Utils/Log.h>
#include <XBook/Debug.h>
#include <IO/IRQ.h>

#ifdef CONFIG_DEBUG
PUBLIC void DebugAssertionFailure(char *exp, char *file, char *baseFile, int line)
{
	LOG_E("\nAssert(%s) failed:\nfile: %s\nbase_file: %s\nln: %d", 
        exp, file, baseFile, line);
	while (1);
}
#endif

PUBLIC void DebugSpin(const char *str)
{
    LOG_I(str, Endln "FILE:%s\nFUNCTION:%s\nLINE:%d", __FILE__, __FUNCTION__, __LINE__);
    while (1);
}

/**
 * OS Panic
 */
PUBLIC void DebugPanic(const char *str)
{
    INTR_Disable();
    LOG_E("!PANIC!");
    SPIN(str);
}
