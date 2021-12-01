/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: IO Interrupt ReQuest 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-28     JasonHu           Init
 */

#ifndef __IO_IRQ__
#define __IO_IRQ__

#include <XBook.h>
#include <Utils/List.h>
#include <XBook/Atomic.h>

#ifdef CONFIG_IRQ_NAME_LEN
#define IRQ_NAME_LEN CONFIG_IRQ_NAME_LEN
#else
#define IRQ_NAME_LEN 48
#endif

#ifndef CONFIG_NR_IRQS
#error "Please config NR_IRQS"
#else
#define NR_IRQS CONFIG_NR_IRQS
#endif

#define IRQ_FLAG_REENTER    0x01    /* handle irq allow reenter */
#define IRQ_FLAG_SHARED     0x02    /* irq was shared by more device */

typedef U32 IRQ_Number;
typedef OS_Error (*IRQ_Handler)(IRQ_Number, void *);

struct IRQ_Controller
{
    OS_Error (*unmask)(IRQ_Number irqno);
    OS_Error (*mask)(IRQ_Number irqno);
    OS_Error (*ack)(IRQ_Number irqno);
    
    void (*enable)(void);
    void (*disable)(void);
    Uint (*saveLevel)(void);
    void (*restoreLevel)(Uint level);
};
typedef struct IRQ_Controller IRQ_Controller;

struct IRQ_Action
{
    List list;
    IRQ_Handler handler;
    void *data;
    U32 flags;
    char name[IRQ_NAME_LEN];
};
typedef struct IRQ_Action IRQ_Action;

struct IRQ_Node
{
    IRQ_Controller *controller;
    List actionList;
    U32 flags;
    Atomic reference;   /* irq reference */
};
typedef struct IRQ_Node IRQ_Node;

INTERFACE IMPORT IRQ_Controller IRQ_ControllerInterface;

PUBLIC OS_Error IRQ_Bind(IRQ_Number irqno,
                         IRQ_Handler handler,
                         void *data,
                         char *name,
                         U32 flags);
                         
PUBLIC OS_Error IRQ_Unbind(IRQ_Number irqno, void *data);

PUBLIC OS_Error IRQ_Unmask(IRQ_Number irqno);
PUBLIC OS_Error IRQ_Mask(IRQ_Number irqno);

PUBLIC OS_Error IRQ_Handle(IRQ_Number irqno);

#define IRQ_Enable()            IRQ_ControllerInterface.enable()
#define IRQ_Disable()           IRQ_ControllerInterface.disable()
#define IRQ_SaveLevel()         IRQ_ControllerInterface.saveLevel()
#define IRQ_RestoreLevel(level) IRQ_ControllerInterface.restoreLevel(level)

PUBLIC void IRQ_Init(void);

#endif  /* __IO_IRQ__ */
