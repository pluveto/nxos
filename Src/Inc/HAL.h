/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Hardware Abstraction Layer
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

#ifndef __XBOOK_HAL__
#define __XBOOK_HAL__

#include <XBook.h>

/* IRQ hal */
typedef OS_Error (*IRQ_Handler)(U32 irqno, void *arg);
typedef U32 IRQ_Number;

struct IRQ_Info
{
    IRQ_Handler handler;
    void *arg;
    char name[IRQ_NAME_LEN];
};
typedef struct IRQ_Info IRQ_Info;

INTERFACE OS_Error HAL_IRQInstall(IRQ_Number irqno, IRQ_Handler handler, void *arg, char *name);
INTERFACE OS_Error HAL_IRQUnInstall(IRQ_Number irqno);
INTERFACE OS_Error HAL_IRQEnable(IRQ_Number irqno);
INTERFACE OS_Error HAL_IRQDisable(IRQ_Number irqno);
INTERFACE OS_Error HAL_IRQAck(IRQ_Number irqno);

INTERFACE void HAL_InterruptEnable(void);
INTERFACE void HAL_InterruptDisable(void);

/* Conosle hal */
INTERFACE void HAL_ConsoleOutChar(char ch);

#endif  /* __XBOOK_HAL__ */