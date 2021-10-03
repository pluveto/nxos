/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Memory utils
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-3      JasonHu           Init
 */

#ifndef __UTILS_MEMORY__
#define __UTILS_MEMORY__

#include <XBook.h>

PUBLIC void *Set(void *dst, u8 value, size sz);
PUBLIC void Copy(void *dst, void *src, size sz);
PUBLIC void *Zero(void *dst, size sz);

#endif  /* __UTILS_MEMORY__ */