/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: HAL Atomic  
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-1      JasonHu           Init
 */

#include <XBook/Atomic.h>

PRIVATE void HAL_AtomicSet(Atomic *atomic, long value)
{
    atomic->value = value;
}

PRIVATE long HAL_AtomicGet(Atomic *atomic)
{
    return atomic->value;
}

PRIVATE void HAL_AtomicAdd(Atomic *atomic, long value)
{
    /* gcc build-in functions */
    __sync_fetch_and_add(&atomic->value, value);
}

PRIVATE void HAL_AtomicSub(Atomic *atomic, long value)
{
    __sync_fetch_and_sub(&atomic->value, value);
}

PRIVATE void HAL_AtomicInc(Atomic *atomic)
{
    __sync_fetch_and_add(&atomic->value, 1);
}

PRIVATE void HAL_AtomicDec(Atomic *atomic)
{
    __sync_fetch_and_sub(&atomic->value, 1);
}

PRIVATE void HAL_AtomicSetMask(Atomic *atomic, long mask)
{
    __sync_fetch_and_or(&atomic->value, mask);
}

PRIVATE void HAL_AtomicClearMask(Atomic *atomic, long mask)
{    
    __sync_fetch_and_and(&atomic->value, ~mask);
}

PRIVATE long HAL_AtomicSwap(Atomic *atomic, long newValue)
{
    return __sync_lock_test_and_set(&((atomic)->value), newValue);
}

PRIVATE long HAL_AtomicCAS(Atomic *atomic, long old, long newValue)
{
    return __sync_val_compare_and_swap(&atomic->value, old, newValue);
}

INTERFACE struct AtomicOps AtomicOpsInterface = 
{
    .set        = HAL_AtomicSet,
    .get        = HAL_AtomicGet,
    .add        = HAL_AtomicAdd,
    .sub        = HAL_AtomicSub,
    .inc        = HAL_AtomicInc,
    .dec        = HAL_AtomicDec,
    .setMask    = HAL_AtomicSetMask,
    .clearMask  = HAL_AtomicClearMask,
    .swap       = HAL_AtomicSwap,
    .cas        = HAL_AtomicCAS,
};
