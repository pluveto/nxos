/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: String utils
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-3      JasonHu           Init
 */

#ifndef __UTILS_STRING__
#define __UTILS_STRING__

#include <XBook.h>
#include <Utils/VarArgs.h>

PUBLIC char *StrCopy(const char *dst, const char *src);
PUBLIC int StrCmp(const char *a, const char *b);
PUBLIC int StrLen(const char *str);
PUBLIC char *StrCopyN(const char *dst, const char *src, Size len);

PUBLIC int VarStrPrintfN(char *buf, int bufLen, const char *fmt, VarList args);
PUBLIC int StrPrintfN(char *buf, int buflen, const char *fmt, ...);

#endif  /* __UTILS_STRING__ */