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
    VOLATILE Int value;
};
typedef struct Atomic Atomic;

#define ATOMIC_INIT_VALUE(val) {val}
#define STATIC_ATOMIC_INIT(name) Atomic name = ATOMIC_INIT_VALUE(0);

struct AtomicOps
{
    void (*set)(Atomic *atomic, Int value);
    Int (*get)(Atomic *atomic);
    void (*add)(Atomic *atomic, Int value);
    void (*sub)(Atomic *atomic, Int value);
    void (*inc)(Atomic *atomic);
    void (*dec)(Atomic *atomic);
    void (*setMask)(Atomic *atomic, Int mask);
    void (*clearMask)(Atomic *atomic, Int mask);
    Int (*swap)(Atomic *atomic, Int newValue);
    Int (*cas)(Atomic *atomic, Int old, Int newValue);
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
