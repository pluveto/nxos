/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Debug system
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-31     JasonHu           Init
 * 2022-1-3       JasonHu           Move to XBook dir
 */

#ifndef __XBOOK_DEBUG__
#define __XBOOK_DEBUG__

#include <XBook.h>

#ifdef CONFIG_DEBUG

PUBLIC void DebugAssertionFailure(char *exp, char *file, char *baseFile, int line);

#define ASSERT(exp) \
    do \
    { \
        if (!(exp)) \
        { \
            DebugAssertionFailure(#exp, __FILE__, __BASE_FILE__, __LINE__); \
        } \
    } while(0)
#else
#define ASSERT(exp)
#endif

/**
 * Spin here
 */
PUBLIC void DebugSpin(const char *str);
#define SPIN(str) DebugSpin(str)

/**
 * OS Panic
 */
PUBLIC void DebugPanic(const char *str);
#define PANIC(str) DebugPanic(str)

#endif  /* __XBOOK_DEBUG__ */
