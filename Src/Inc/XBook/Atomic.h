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

struct Atomic
{
    VOLATILE long value;
};
typedef struct Atomic Atomic;

struct AtomicOps
{
    void (*set)(Atomic *atomic, long value);
    long (*get)(Atomic *atomic);
    void (*add)(Atomic *atomic, long value);
    void (*sub)(Atomic *atomic, long value);
    void (*inc)(Atomic *atomic);
    void (*dec)(Atomic *atomic);
    void (*setMask)(Atomic *atomic, long mask);
    void (*clearMask)(Atomic *atomic, long mask);
    long (*swap)(Atomic *atomic, long newValue);
    long (*cas)(Atomic *atomic, long old, long newValue);
};

INTERFACE IMPORT struct AtomicOps AtomicOpsInterface;

#define AtomicSet(atomic, value)    AtomicOpsInterface.set(atomic, value)
#define AtomicGet(atomic)           AtomicOpsInterface.get(atomic)
#define AtomicAdd(atomic, value)    AtomicOpsInterface.add(atomic, value)
#define AtomicSub(atomic, value)    AtomicOpsInterface.sub(atomic, value)
#define AtomicInc(atomic)           AtomicOpsInterface.inc(atomic)
#define AtomicDec(atomic)           AtomicOpsInterface.dec(atomic)
#define AtomicSetMask(atomic, mask)    AtomicOpsInterface.setMask(atomic, mask)
#define AtomicClearMask(atomic, mask)    AtomicOpsInterface.clearMask(atomic, mask)
#define AtomicSwap(atomic, newValue)    AtomicOpsInterface.swap(atomic, newValue)
#define AtomicCAS(atomic, old, newValue)    AtomicOpsInterface.cas(atomic, old, newValue)

#endif /* __XBOOK_ATOMIC__ */
