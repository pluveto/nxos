/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Direct uart
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-12-4      JasonHu           Init
 */

#ifndef __DIRECT_UART_HEADER__
#define __DIRECT_UART_HEADER__

#include <XBook.h>

#define UART0_PHY_ADDR 0x10000000L
#define UART0_IRQ 10

/* direct means not use driver framework */

PUBLIC void HAL_DirectUartInit(void);
PUBLIC void HAL_DirectUartStage2(void);

PUBLIC void HAL_DirectUartPutc(char ch);
PUBLIC int HAL_DirectUartGetc(void);

#endif /* __DIRECT_UART_HEADER__ */
