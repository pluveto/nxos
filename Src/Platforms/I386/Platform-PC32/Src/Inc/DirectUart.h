/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Direct uart driver header
 * 
 * Change Logs:
 * Date           Author        Notes
 * 2021-07-15     JasonHu       first version
 */

#ifndef __DIRECT_UART_HEADER__
#define __DIRECT_UART_HEADER__

#include <Defines.h>

/* direct means not use driver framework */

PUBLIC void HAL_DirectUartInit(void);
PUBLIC void HAL_DirectUartPutc(char ch);

#endif /* __DIRECT_UART_HEADER__ */
