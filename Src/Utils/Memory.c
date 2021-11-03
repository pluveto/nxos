/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Memory tools
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-3      JasonHu           Init
 */

#include <Utils/Memory.h>

PUBLIC void *Set(void *dst, U8 value, Size sz)
{
    U8 *dstPtr = (U8 *)dst;
	while (sz > 0)
    {
		*dstPtr++ = value;
		--sz;
	}
	return dst;
}

PUBLIC void Copy(void *dst, void *src, Size sz)
{
    U8 *dstPtr = (U8 *)dst;
    U8 *srcPtr = (U8 *)src;
    while (sz-- > 0)
    {
        *dstPtr++ = *srcPtr++;
    }
}

PUBLIC void *Zero(void *dst, Size sz)
{
    return Set(dst, 0, sz);
}

PUBLIC int CompareN(const void *s1, const void *s2, Size nBytes)
{
	if ((s1 == 0) || (s2 == 0)) 
    {
		return (s1 - s2);
	}

	const char *p1 = (const char *)s1;
	const char *p2 = (const char *)s2;
	int i;
	for (i = 0; i < nBytes; i++,p1++,p2++)
    {
		if (*p1 != *p2)
        {
			return (*p1 - *p2);
		}
	}
	return 0;
}
