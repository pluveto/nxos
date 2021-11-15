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

/* Platform */
INTERFACE OS_Error PlatformInit(void);

/* IRQ HAL */
typedef OS_Error (*IRQ_Handler)(U32 irqno, void *arg);
typedef U32 IRQ_Number;

#ifdef CONFIG_IRQ_NAME_LEN
#define IRQ_NAME_LEN CONFIG_IRQ_NAME_LEN
#else
#define IRQ_NAME_LEN 48
#endif

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
INTERFACE Uint HAL_InterruptSaveLevel(void);
INTERFACE void HAL_InterruptRestoreLevel(Uint level);

/* Conosle HAL */
INTERFACE void HAL_ConsoleOutChar(char ch);

/* Context HAL */
INTERFACE U8 *HAL_ContextInit(void *entry, void *arg, U8 *stackBase, void *exit);
INTERFACE void HAL_ContextSwitchNext(Addr nextSP);
INTERFACE void HAL_ContextSwitchPrevNext(Addr prevSP, Addr nextSP);

#endif  /* __XBOOK_HAL__ */