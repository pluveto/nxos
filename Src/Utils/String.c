/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: string tools 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-3      JasonHu           Init
 */

#include <Utils/String.h>

PUBLIC char *StrCopy(char *dst, char *src)
{
    char *dstPtr = dst;		       
    while ((*dstPtr++ = *src++))
    {
    }
    return dst;
}

PUBLIC char StrCmp(const char* a, const char* b)
{  
    while (*a != 0 && *a == *b)
    {
        a++;
        b++;
    }
    return *a < *b ? -1 : *a > *b;
}

PUBLIC int StrLen(const char *str)
{
    const char *p = str;
    while(*p++);
    return (p - str - 1);
}
