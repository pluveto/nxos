/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Direct uart
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-3      JasonHu           Init
 */

#ifndef __DIRECT_UART_HEADER__
#define __DIRECT_UART_HEADER__

#include <XBook.h>

/* direct means not use driver framework */

NX_PUBLIC void HAL_DirectUartInit(void);
NX_PUBLIC void HAL_DirectUartPutc(char ch);
NX_PUBLIC void HAL_DirectUartStage2(void);

#endif /* __DIRECT_UART_HEADER__ */
