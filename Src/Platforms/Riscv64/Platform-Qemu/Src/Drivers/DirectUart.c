/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Direct uart driver 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-1      JasonHu           Init
 */

#include <XBook.h>
#include <HAL.h>
#include <SBI.h>

INTERFACE void HAL_ConsoleOutChar(char ch)
{
    SBI_ConsolePutChar(ch);
}
