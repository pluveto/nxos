/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Debug utils
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-31     JasonHu           Init
 */

#ifndef __UTILS_DEBUG__
#define __UTILS_DEBUG__

#include <XBook.h>
#include <Utils/Log.h>

#ifdef CONFIG_DEBUG
void AssertionFailure(char *exp, char *file, char *baseFile, int line);
#define ASSERT(exp)  if (exp) ; \
        else AssertionFailure(#exp, __FILE__, __BASE_FILE__, __LINE__)
#else
#define ASSERT(exp)
#endif

void AssertionFailure(char *exp, char *file, char *baseFile, int line);

/**
 * Spin here
 */
#define SPIN(str) \
        LOG_I(str, Endln "FILE:" $s(__FILE__) "\nFUNCTION:" $s(__FUNCTION__) "\nLINE:" $d(__LINE__)); \
        for(;;)

/**
 * OS Panic
 */
#define PANIC(str) \
        HAL_InterruptDisable(); \
        LOG_E("!PANIC!" CON_NEWLINE); \
        SPIN(str)

#endif  /* __UTILS_DEBUG__ */
