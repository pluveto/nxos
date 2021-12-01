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

PRIVATE void HAL_AtomicSet(Atomic *atomic, int value)
{
    atomic->value = value;
}

PRIVATE int HAL_AtomicGet(Atomic *atomic)
{
    return atomic->value;
}

PRIVATE void HAL_AtomicAdd(Atomic *atomic, int value)
{
    
}

PRIVATE void HAL_AtomicSub(Atomic *atomic, int value)
{
    
}

PRIVATE void HAL_AtomicInc(Atomic *atomic)
{
    
}

PRIVATE void HAL_AtomicDec(Atomic *atomic)
{
      
}

PRIVATE void HAL_AtomicSetMask(Atomic *atomic, int mask)
{
    
}

PRIVATE void HAL_AtomicClearMask(Atomic *atomic, int mask)
{    
    
}

PRIVATE int HAL_AtomicSwap(Atomic *atomic, int newValue)
{
    
    return newValue;
}

PRIVATE int HAL_AtomicCAS(Atomic *atomic, int old, int newValue)
{
    int prev = 0;
    return prev;
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
