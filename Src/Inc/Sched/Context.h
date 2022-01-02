/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: context switch
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2022-1-2       JasonHu           Init
 */

#ifndef __SCHED_CONTEXT___
#define __SCHED_CONTEXT___

#include <XBook.h>

struct ContextOps
{
    void *(*init)(void *startEntry, void *exitEntry, void *arg, void *stackTop);
    void (*switchNext)(Addr nextSP);
    void (*switchPrevNext)(Addr prevSP, Addr nextSP);
};
INTERFACE IMPORT struct ContextOps ContextOpsInterface;

#define ContextInit(startEntry, exitEntry, arg, stackTop)   ContextOpsInterface.init(startEntry, exitEntry, arg, stackTop)
#define ContextSwitchNext(nextSP)                           ContextOpsInterface.switchNext(nextSP)
#define ContextSwitchPrevNext(prevSP, nextSP)               ContextOpsInterface.switchPrevNext(prevSP, nextSP)

#endif /* __SCHED_CONTEXT___ */
