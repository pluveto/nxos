/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Atomic
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-11     JasonHu           Init
 */

#ifndef __XBOOK_ATOMIC__
#define __XBOOK_ATOMIC__

#include <XBook.h>

struct HAL_Atomic
{
    VOLATILE int value;
};
typedef struct HAL_Atomic HAL_Atomic;

INTERFACE void HAL_AtomicSet(HAL_Atomic *atomic, int value);
INTERFACE int HAL_AtomicGet(HAL_Atomic *atomic);
INTERFACE void HAL_AtomicAdd(HAL_Atomic *atomic, int value);
INTERFACE void HAL_AtomicSub(HAL_Atomic *atomic, int value);
INTERFACE void HAL_AtomicInc(HAL_Atomic *atomic);
INTERFACE void HAL_AtomicDec(HAL_Atomic *atomic);
INTERFACE void HAL_AtomicSetMask(HAL_Atomic *atomic, int mask);
INTERFACE void HAL_AtomicClearMask(HAL_Atomic *atomic, int mask);
INTERFACE int HAL_AtomicSwap(HAL_Atomic *atomic, int newValue);
INTERFACE int HAL_AtomicCAS(HAL_Atomic *atomic, int old, int newValue);

#endif /* __XBOOK_ATOMIC__ */
