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

#include <IO/DelayIRQ.h>
#include <MM/Alloc.h>

#define IRQ_WORK_PENDING  0x80000000    /* work is pending */

PRIVATE List DelayIrqListTable[IRQ_QUEUE_NR];
PRIVATE U32 DelayIrqEvent;

PUBLIC void IRQ_DelayQueueInit(void)
{
    int i;
    for (i = 0; i < IRQ_QUEUE_NR; i++)
    {
        ListInit(&DelayIrqListTable[i]);
    }
    DelayIrqEvent = 0;
}

PRIVATE void IRQ_DelayEventSet(U32 event)
{
    DelayIrqEvent |= event;
}

PRIVATE void IRQ_DelayEventClear(void)
{
    DelayIrqEvent = 0;
}

PUBLIC OS_Error IRQ_DelayQueueEnter(IRQ_DelayQueue queue, IRQ_DelayWork *work)
{
    if (queue < 0 || queue >= IRQ_QUEUE_NR || work == NULL)
    {
        return OS_EINVAL;
    }
    
    if (ListFind(&work->list, &DelayIrqListTable[queue]))
    {
        return OS_EAGAIN;
    }

    Uint level = IRQ_SaveLevel();
    work->queue = queue;
    ListAddTail(&work->list, &DelayIrqListTable[queue]);
    IRQ_RestoreLevel(level);
    return OS_EOK;
}

PUBLIC OS_Error IRQ_DelayQueueLeave(IRQ_DelayQueue queue, IRQ_DelayWork *work)
{
    if (queue < 0 || queue >= IRQ_QUEUE_NR || work == NULL)
    {
        return OS_EINVAL;
    }
    
    if (!ListFind(&work->list, &DelayIrqListTable[queue]))
    {
        return OS_ENOSRCH;
    }

    Uint level = IRQ_SaveLevel();
    work->queue = 0;
    ListDel(&work->list);
    IRQ_RestoreLevel(level);
    return OS_EOK;
}

PUBLIC OS_Error IRQ_DelayWorkInit(IRQ_DelayWork *work, IRQ_WorkHandler handler, void *arg, U32 flags)
{
    if (work == NULL || handler == NULL)
    {
        return OS_EINVAL;
    }
    work->handler = handler;
    work->arg = arg;
    work->flags = flags;
    work->flags &= ~IRQ_WORK_NOREENTER; /*  */
    ListInit(&work->list);
    return OS_EOK;
}

PUBLIC IRQ_DelayWork *IRQ_DelayWorkCreate(IRQ_WorkHandler handler, void *arg, U32 flags)
{
    IRQ_DelayWork *work = MemAlloc(sizeof(IRQ_DelayWork));
    if (work == NULL)
    {
        return NULL;
    }
    if (IRQ_DelayWorkInit(work, handler, arg, flags) != OS_EOK)
    {
        MemFree(work);
        return NULL;
    }
    return work;
}

PUBLIC OS_Error IRQ_DelayWorkDestroy(IRQ_DelayWork *work)
{
    if (work == NULL)
    {
        return OS_EINVAL;
    }
    MemFree(work);
    return OS_EOK;
}

/**
 * Must called with interrupt disabled
 */
PUBLIC OS_Error IRQ_DelayWorkHandle(IRQ_DelayWork *work)
{
    if (work == NULL)
    {
        return OS_EINVAL;
    }
    if (work->queue < 0 || work->queue >= IRQ_QUEUE_NR)
    {
        return OS_EFAULT;
    }
    IRQ_DelayEventSet((1 << work->queue));
    work->flags |= IRQ_WORK_PENDING;

    return OS_EOK;
}

INLINE void IRQ_DelayWorkCheck(IRQ_DelayWork *work)
{
    if (work->flags & IRQ_WORK_PENDING)
    {
        work->flags &= ~IRQ_WORK_PENDING; /* clear pending */

        if (!(work->flags & IRQ_WORK_NOREENTER))
        {
            IRQ_Enable();   
        }

        work->handler(work->arg);

        if (!(work->flags & IRQ_WORK_NOREENTER))
        {
            IRQ_Disable();                      
        }
    }
}

/**
 * Must called interrupt disabled
 */
INTERFACE void IRQ_DelayQueueCheck(void)
{
    int checkTimes = IRQ_DELAY_WORK_CHECK_TIMES;
    
    while (checkTimes-- > 0)
    {
        U32 irqEvent = DelayIrqEvent;
        IRQ_DelayEventClear();

        if (irqEvent == 0)
        {
            return;
        }
        IRQ_Enable();

        int i;
        for (i = 0; i < IRQ_QUEUE_NR; i++)
        {
            if (!(irqEvent & (1 << i)))
            {
                continue;
            }
            IRQ_DelayWork *work;
            ListForEachEntry(work, &DelayIrqListTable[i], list)
            {
                IRQ_DelayWorkCheck(work);
            }
        }
        IRQ_Disable();
    }
}
