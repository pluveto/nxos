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

#include <Defines.h>

/* direct means not use driver framework */

PUBLIC void HAL_DirectUartInit(void);
PUBLIC void HAL_DirectUartPutc(char ch);

#endif /* __DIRECT_UART_HEADER__ */
