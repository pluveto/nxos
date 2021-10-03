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

PUBLIC void *Set(void *dst, u8 value, size sz)
{
    u8 *dstPtr = (u8 *)dst;
	while (sz > 0)
    {
		*dstPtr++ = value;
		--sz;
	}
	return dst;
}

PUBLIC void Copy(void *dst, void *src, size sz)
{
    u8 *dstPtr = (u8 *)dst;
    u8 *srcPtr = (u8 *)src;
    while (sz-- > 0)
    {
        *dstPtr++ = *srcPtr++;
    }
}

PUBLIC void *Zero(void *dst, size sz)
{
    return Set(dst, 0, sz);
}
