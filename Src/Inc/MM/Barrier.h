/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: memory barrier
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-7      JasonHu           Init
 */

#ifndef __MEMORY_BARRIER__
#define __MEMORY_BARRIER__

#include <XBook.h>

struct MemBarrierOps
{
    void (*barrier)(void);
    void (*barrierRead)(void);
    void (*barrierWrite)(void);
    void (*barrierInstruction)(void);
};

INTERFACE IMPORT struct MemBarrierOps MemBarrierOpsInterface;

#define MemoryBarrier MemBarrierOpsInterface.barrier
#define MemoryBarrierRead MemBarrierOpsInterface.barrierRead
#define MemoryBarrierWrite MemBarrierOpsInterface.barrierWrite
#define MemoryBarrierInstruction MemBarrierOpsInterface.barrierInstruction

#endif /* __MEMORY_BARRIER__ */
