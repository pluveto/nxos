/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Assert for OS 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-17     JasonHu           Init
 */

#ifndef __XBOOK_ASEERT__
#define __XBOOK_ASEERT__

#include <XBook.h>
#include <Mods/Console/Console.h>

#ifdef CONFIG_ASSERT_DEBUG
void AssertionFailure(char *exp, char *file, char *baseFile, int line);
#define ASSERT(exp)  if (exp) ; \
        else AssertionFailure(#exp, __FILE__, __BASE_FILE__, __LINE__)
#else
#define ASSERT(exp)
#endif

#endif /* __XBOOK_ASEERT__ */
