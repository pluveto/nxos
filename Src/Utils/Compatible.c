/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Compatiable functions
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-3      JasonHu           Init
 * 2021-1-6       JasonHu           move to compatible
 * 
 */

#include <XBook.h>

/**
 * compatiable for gcc compiler with optimize
 */
NX_PUBLIC void *memset(void *dst, NX_U8 value, NX_USize sz)
{
    NX_U8 *dstPtr = (NX_U8 *)dst;
	while (sz > 0)
    {
		*dstPtr++ = value;
		--sz;
	}
	return dst;
}
