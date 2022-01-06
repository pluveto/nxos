/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: HAL NX_Atomic  
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-1      JasonHu           Init
 */

#include <XBook/Atomic.h>

NX_PRIVATE void HAL_AtomicSet(NX_Atomic *atomic, long value)
{
    atomic->value = value;
}

NX_PRIVATE long HAL_AtomicGet(NX_Atomic *atomic)
{
    return atomic->value;
}

NX_PRIVATE void HAL_AtomicAdd(NX_Atomic *atomic, long value)
{
    /* gcc build-in functions */
    __sync_fetch_and_add(&atomic->value, value);
}

NX_PRIVATE void HAL_AtomicSub(NX_Atomic *atomic, long value)
{
    __sync_fetch_and_sub(&atomic->value, value);
}

NX_PRIVATE void HAL_AtomicInc(NX_Atomic *atomic)
{
    __sync_fetch_and_add(&atomic->value, 1);
}

NX_PRIVATE void HAL_AtomicDec(NX_Atomic *atomic)
{
    __sync_fetch_and_sub(&atomic->value, 1);
}

NX_PRIVATE void HAL_AtomicSetMask(NX_Atomic *atomic, long mask)
{
    __sync_fetch_and_or(&atomic->value, mask);
}

NX_PRIVATE void HAL_AtomicClearMask(NX_Atomic *atomic, long mask)
{    
    __sync_fetch_and_and(&atomic->value, ~mask);
}

NX_PRIVATE long HAL_AtomicSwap(NX_Atomic *atomic, long newValue)
{
    return __sync_lock_test_and_set(&((atomic)->value), newValue);
}

NX_PRIVATE long HAL_AtomicCAS(NX_Atomic *atomic, long old, long newValue)
{
    return __sync_val_compare_and_swap(&atomic->value, old, newValue);
}

NX_INTERFACE struct NX_AtomicOps NX_AtomicOpsInterface = 
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
