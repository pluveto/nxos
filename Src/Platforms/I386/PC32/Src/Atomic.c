/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Atomic for x86 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-11     JasonHu           Init
 */

#include <XBook/Atomic.h>

#define LOCK_PREFIX "lock "

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
    CASM(LOCK_PREFIX "addl %1,%0"   
         : "+m" (atomic->value)   
         : "ir" (value));
}

PRIVATE void HAL_AtomicSub(Atomic *atomic, long value)
{
    CASM(LOCK_PREFIX "subl %1,%0"   
         : "+m" (atomic->value)   
         : "ir" (value));
}

PRIVATE void HAL_AtomicInc(Atomic *atomic)
{
    CASM(LOCK_PREFIX "incl %0"   
         : "+m" (atomic->value));
}

PRIVATE void HAL_AtomicDec(Atomic *atomic)
{
    CASM(LOCK_PREFIX "decl %0"   
         : "+m" (atomic->value));   
}

PRIVATE void HAL_AtomicSetMask(Atomic *atomic, long mask)
{
    CASM(LOCK_PREFIX "orl %0,%1"
        : : "r" (mask), "m" (*(&atomic->value)) : "memory");
}

PRIVATE void HAL_AtomicClearMask(Atomic *atomic, long mask)
{    
    CASM(LOCK_PREFIX "andl %0,%1"
         : : "r" (~(mask)), "m" (*(&atomic->value)) : "memory");
}

PRIVATE long HAL_AtomicSwap(Atomic *atomic, long newValue)
{
    CASM("xchgl %k0,%1"   
         : "=r" (newValue)
         : "m" (*(&atomic->value)), "0" (newValue)   
         : "memory");
    return newValue;
}

PRIVATE long HAL_AtomicCAS(Atomic *atomic, long old, long newValue)
{
    long prev;
    CASM(LOCK_PREFIX "cmpxchgl %k1,%2"   
         : "=a"(prev)
         : "r"(newValue), "m"(*(&atomic->value)), "0"(old)   
         : "memory");
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
