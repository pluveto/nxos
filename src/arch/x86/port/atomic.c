/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: NX_Atomic for x86 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-11     JasonHu           Init
 */

#include <xbook/atomic.h>

#define LOCK_PREFIX "lock "

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
    NX_CASM(LOCK_PREFIX "addl %1,%0"   
         : "+m" (atomic->value)   
         : "ir" (value));
}

NX_PRIVATE void HAL_AtomicSub(NX_Atomic *atomic, long value)
{
    NX_CASM(LOCK_PREFIX "subl %1,%0"   
         : "+m" (atomic->value)   
         : "ir" (value));
}

NX_PRIVATE void HAL_AtomicInc(NX_Atomic *atomic)
{
    NX_CASM(LOCK_PREFIX "incl %0"   
         : "+m" (atomic->value));
}

NX_PRIVATE void HAL_AtomicDec(NX_Atomic *atomic)
{
    NX_CASM(LOCK_PREFIX "decl %0"   
         : "+m" (atomic->value));   
}

NX_PRIVATE void HAL_AtomicSetMask(NX_Atomic *atomic, long mask)
{
    NX_CASM(LOCK_PREFIX "orl %0,%1"
        : : "r" (mask), "m" (*(&atomic->value)) : "memory");
}

NX_PRIVATE void HAL_AtomicClearMask(NX_Atomic *atomic, long mask)
{    
    NX_CASM(LOCK_PREFIX "andl %0,%1"
         : : "r" (~(mask)), "m" (*(&atomic->value)) : "memory");
}

NX_PRIVATE long HAL_AtomicSwap(NX_Atomic *atomic, long newValue)
{
    NX_CASM("xchgl %k0,%1"   
         : "=r" (newValue)
         : "m" (*(&atomic->value)), "0" (newValue)   
         : "memory");
    return newValue;
}

NX_PRIVATE long HAL_AtomicCAS(NX_Atomic *atomic, long old, long newValue)
{
    long prev;
    NX_CASM(LOCK_PREFIX "cmpxchgl %k1,%2"   
         : "=a"(prev)
         : "r"(newValue), "m"(*(&atomic->value)), "0"(old)   
         : "memory");
    return prev;
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
