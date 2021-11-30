/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: IO Interrupt ReQuest Delay Queue 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-29     JasonHu           Init
 */

#ifndef __IO_DELAY_IRQ__
#define __IO_DELAY_IRQ__

#include <XBook.h>
#include <Utils/List.h>
#include <IO/IRQ.h>

enum IRQ_DelayQueue 
{
    IRQ_FAST_QUEUE = 0,
    IRQ_NORMAL_QUEUE,
    IRQ_PERIOD_QUEUE,
    IRQ_SCHED_QUEUE,
    IRQ_SLOW_QUEUE,
    IRQ_QUEUE_NR,
};
typedef enum IRQ_DelayQueue IRQ_DelayQueue;

#define IRQ_WORK_NOREENTER  0x01    /* no reenter when do work */

#define IRQ_DELAY_WORK_CHECK_TIMES 5

typedef void (*IRQ_WorkHandler)(void *arg);

struct IRQ_DelayWork
{
    List list;
    U32 flags;
    IRQ_WorkHandler handler;
    void *arg;
    IRQ_DelayQueue queue;
};
typedef struct IRQ_DelayWork IRQ_DelayWork;

PUBLIC void IRQ_DelayQueueInit(void);

PUBLIC OS_Error IRQ_DelayQueueEnter(IRQ_DelayQueue queue, IRQ_DelayWork *work);
PUBLIC OS_Error IRQ_DelayQueueLeave(IRQ_DelayQueue queue, IRQ_DelayWork *work);

PUBLIC IRQ_DelayWork *IRQ_DelayWorkCreate(IRQ_WorkHandler handler, void *arg, U32 flags);
PUBLIC OS_Error IRQ_DelayWorkDestroy(IRQ_DelayWork *work);
PUBLIC OS_Error IRQ_DelayWorkInit(IRQ_DelayWork *work, IRQ_WorkHandler handler, void *arg, U32 flags);

PUBLIC OS_Error IRQ_DelayWorkHandle(IRQ_DelayWork *work);

INTERFACE void IRQ_DelayQueueCheck(void);

#endif  /* __IO_DELAY_IRQ__ */
