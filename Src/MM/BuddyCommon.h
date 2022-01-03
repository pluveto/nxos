/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Buddy system common
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-17     JasonHu           Update code style
 */

#define LOG_LEVEL LOG_INFO
#define LOG_NAME "Buddy"
#include <Utils/Debug.h>
#include <Utils/Bitops.h>
#include <XBook.h>

#define BUDDY_ASSERT(x, msg) ASSERT((x) && (msg))

/**
 * find first bit set
 */
INLINE int BuddyFFS(U32 word)
{
    return FFS(word) - 1;
}

/**
 * find last bit set
 */
INLINE int BuddyFLS(U32 word)
{
    return FLS(word) - 1;
}

/* Possibly 64-bit version of BuddyFLS. */
#if defined(CONFIG_CPU_64BITS)
PRIVATE int BuddyFlsSizet(USize size)
{
    int high = (int)(size >> 32);
    int bits = 0;
    if (high)
    {
        bits = 32 + BuddyFLS(high);
    }
    else
    {
        bits = BuddyFLS((int)size & 0xffffffff);
    }
    return bits;
}

PRIVATE int BuddyFfsSizet(USize size)
{
    int low = (int)(size);
    int bits = 0;
    if (low)
    {
        bits = BuddyFFS(low);
    }
    else
    {
        bits = 32 + BuddyFFS((int)(size >> 32));
    }
    return bits;
}
#else
#define BuddyFlsSizet BuddyFLS
#define BuddyFfsSizet BuddyFFS
#endif

PRIVATE USize BuddyAlignUp(USize x, USize align)
{
    BUDDY_ASSERT(0 == (align & (align - 1)), "must align to a power of two");
    return (x + (align - 1)) & ~(align - 1);
}

USED PRIVATE USize BuddyAlignDown(USize x, USize align)
{
    BUDDY_ASSERT(0 == (align & (align - 1)), "must align to a power of two");
    return x - (x & (align - 1));
}

PRIVATE void *BuddyAlignPtr(const void *ptr, USize align)
{
    ASSERT(ptr && align);

    const PtrDiff aligned =
        (TypeCast(PtrDiff, ptr) + (align - 1)) & ~(align - 1);
    BUDDY_ASSERT(0 == (align & (align - 1)), "must align to a power of two");
    return TypeCast(void*, aligned);
}
