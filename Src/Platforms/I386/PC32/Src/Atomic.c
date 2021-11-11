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

INTERFACE void HAL_AtomicSet(HAL_Atomic *atomic, int value)
{
    atomic->value = value;
}

INTERFACE int HAL_AtomicGet(HAL_Atomic *atomic)
{
    return atomic->value;
}

INTERFACE void HAL_AtomicAdd(HAL_Atomic *atomic, int value)
{
    CASM(LOCK_PREFIX "addl %1,%0"   
         : "+m" (atomic->value)   
         : "ir" (value));
}

INTERFACE void HAL_AtomicSub(HAL_Atomic *atomic, int value)
{
    CASM(LOCK_PREFIX "subl %1,%0"   
         : "+m" (atomic->value)   
         : "ir" (value));
}

INTERFACE void HAL_AtomicInc(HAL_Atomic *atomic)
{
    CASM(LOCK_PREFIX "incl %0"   
         : "+m" (atomic->value));
}

INTERFACE void HAL_AtomicDec(HAL_Atomic *atomic)
{
    CASM(LOCK_PREFIX "decl %0"   
         : "+m" (atomic->value));   
}

INTERFACE void HAL_AtomicSetMask(HAL_Atomic *atomic, int mask)
{
    CASM(LOCK_PREFIX "orl %0,%1"
        : : "r" (mask), "m" (*(&atomic->value)) : "memory");
}

INTERFACE void HAL_AtomicClearMask(HAL_Atomic *atomic, int mask)
{    
    CASM(LOCK_PREFIX "andl %0,%1"
         : : "r" (~(mask)), "m" (*(&atomic->value)) : "memory");
}

INTERFACE int HAL_AtomicSwap(HAL_Atomic *atomic, int newValue)
{
    CASM("xchgl %k0,%1"   
         : "=r" (newValue)
         : "m" (*(&atomic->value)), "0" (newValue)   
         : "memory");
    return newValue;
}

INTERFACE int HAL_AtomicCAS(HAL_Atomic *atomic, int old, int newValue)
{
    int prev;
    CASM(LOCK_PREFIX "cmpxchgl %k1,%2"   
         : "=a"(prev)
         : "r"(newValue), "m"(*(&atomic->value)), "0"(old)   
         : "memory");
    return prev;
}
