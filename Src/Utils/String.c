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
#include <Utils/Log.h>

PUBLIC char *StrCopy(const char *dst, const char *src)
{
    if (dst == NULL || src == NULL)
    {
        LOG_D("NULL arg: %s", __func__);
        return NULL;
    }
    char *dstPtr = (char *) dst;
    while ((*dstPtr++ = *src++))
    {
    }
    return (char *)dst;
}

PUBLIC char StrCmp(const char *a, const char *b)
{
    if (a == NULL || b == NULL)
    {
        LOG_D("NULL arg: %s", __func__);
        return 0;
    }
    while (*a != 0 && *a == *b)
    {
        a++;
        b++;
    }
    return *a < *b ? -1 : *a > *b;
}

PUBLIC int StrLen(const char *str)
{
    if (str == NULL)
    {
        LOG_D("NULL arg: %s", __func__);
        return 0;
    }
    const char *p = str;
    while(*p++);
    return (p - str - 1);
}
