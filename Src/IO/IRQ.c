/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: IRQ
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-28     JasonHu           Init
 */

#include <IO/IRQ.h>
#include <IO/DelayIRQ.h>
#include <Utils/Memory.h>
#include <Utils/String.h>
#include <MM/Alloc.h>

PRIVATE IRQ_Node IRQ_NodeTable[NR_IRQS];

PUBLIC void IRQ_Init(void)
{
    int i;
    IRQ_Node *irq;
    for (i = 0; i < NR_IRQS; i++)
    {
        irq = &IRQ_NodeTable[i];
        irq->flags = 0;
        irq->controller = NULL;
        AtomicSet(&irq->reference, 0);
        ListInit(&irq->actionList);
    }
    IRQ_DelayQueueInit();
}

PRIVATE IRQ_Node *IRQ_NodeGet(IRQ_Number irq)
{
    if (0 <= irq && irq < NR_IRQS) {
        return &IRQ_NodeTable[irq];
    }
    return NULL;
}

PUBLIC OS_Error IRQ_Bind(IRQ_Number irqno,
                         IRQ_Handler handler,
                         void *data,
                         char *name,
                         U32 flags)
{
    IRQ_Node *irqNode = IRQ_NodeGet(irqno);
    if (irqNode == NULL)
    {
        return OS_EINVAL;
    }
    irqNode->controller = &IRQ_ControllerInterface;
    irqNode->flags = flags;

    IRQ_Action *action = MemAlloc(sizeof(IRQ_Action));
    if (action == NULL)
    {
        return OS_ENOMEM;
    }
    action->data = data;
    action->flags = flags;
    action->handler = handler;
    Zero(action->name, IRQ_NAME_LEN);
    StrCopyN(action->name, name, IRQ_NAME_LEN);
    
    /* add to action list */
    ListAddTail(&action->list, &irqNode->actionList);
    
    AtomicInc(&irqNode->reference);
    return OS_EOK;
}

PUBLIC OS_Error IRQ_Unbind(IRQ_Number irqno, void *data)
{
    IRQ_Node *irqNode = IRQ_NodeGet(irqno);
    if (irqNode == NULL)
    {
        return OS_EINVAL;
    }
    if (irqNode->controller == NULL)
    {
        return OS_EFAULT;
    }

    IRQ_Action *action = NULL;
    IRQ_Action *actionFind = NULL;

    Uint level = INTR_SaveLevel();
    if (irqNode->flags & IRQ_FLAG_SHARED)
    {
        ListForEachEntry(action, &irqNode->actionList, list)
        {
            /* find irq device if data are same */
            if (action->data == data)
            {
                actionFind = action;
                break;
            }
        }
    }
    else
    {
        actionFind = ListFirstEntryOrNULL(&irqNode->actionList, IRQ_Action, list);
    }
    if (actionFind == NULL)
    {
        INTR_RestoreLevel(level);
        return OS_ENORES;
    }
    /* remove action */
    ListDel(&actionFind->list);
    MemFree(actionFind);

    AtomicDec(&irqNode->reference);
    /* no device on this irq */
    if (AtomicGet(&irqNode->reference) == 0)
    {
        irqNode->controller = NULL;
        irqNode->flags = 0;
    }
    INTR_RestoreLevel(level);
    return OS_EOK;
}

PUBLIC OS_Error IRQ_Unmask(IRQ_Number irqno)
{
    IRQ_Node *irqNode = IRQ_NodeGet(irqno);
    if (irqNode == NULL)
    {
        return OS_EINVAL;
    }
    if (irqNode->controller != NULL && irqNode->controller->unmask != NULL)
    {
        return irqNode->controller->unmask(irqno);
    }
    return OS_ENOFUNC;
}

PUBLIC OS_Error IRQ_Mask(IRQ_Number irqno)
{
    IRQ_Node *irqNode = IRQ_NodeGet(irqno);
    if (irqNode == NULL)
    {
        return OS_EINVAL;
    }
    if (irqNode->controller != NULL && irqNode->controller->mask != NULL)
    {
        return irqNode->controller->mask(irqno);
    }
    return OS_ENOFUNC;
}

PUBLIC OS_Error IRQ_Handle(IRQ_Number irqno)
{
    IRQ_Node *irqNode = IRQ_NodeGet(irqno);
    if (irqNode == NULL)
    {
        return OS_EINVAL;
    }
    IRQ_Action *action;

    /* invoke each action on irq node */
    ListForEachEntry(action, &irqNode->actionList, list)
    {
        if (action->handler(irqno, action->data) == OS_EOK)
        {
            break;
        }
    }
    /* ack irq after handled */
    if (irqNode->controller->ack != NULL)
    {
        irqNode->controller->ack(irqno);
    }
    return OS_EOK;
}
