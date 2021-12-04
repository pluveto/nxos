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
#include <XBook/HAL.h>
#include <SBI.h>
#include <IO/DelayIRQ.h>
#include <Drivers/DirectUart.h>
#include <Utils/Log.h>
#include <Utils/Debug.h>

#if DIRECT_UART_LOOP_MODE == 0
PRIVATE IRQ_DelayWork UartWork;
#endif

INTERFACE void HAL_ConsoleOutChar(char ch)
{
    SBI_ConsolePutChar(ch);
}

PUBLIC void HAL_DirectUartPutc(char ch)
{
    SBI_ConsolePutChar(ch);
}

PUBLIC int HAL_DirectUartTryGetc(void)
{
    return SBI_ConsoleGetChar();
}

PUBLIC void HAL_DirectUartInit(void)
{
    /* do nothing, sbi dose well */
}

#if DIRECT_UART_LOOP_MODE == 0
/**
 * default handler
*/
WEAK_SYM PUBLIC void HAL_DirectUartGetc(char data)
{
    LOG_I("Deafult uart handler:%x/%c\n", data, data);
}

PRIVATE void UartWorkHandler(void *arg)
{
    int data = -1;
    data = HAL_DirectUartTryGetc();
    if (data != -1)
    {
        if (HAL_DirectUartGetc != NULL)
        {
            HAL_DirectUartGetc(data);
        }
    }
}

PRIVATE OS_Error UartIrqHandler(IRQ_Number irqno, void *arg)
{
    IRQ_DelayWorkHandle(&UartWork);
    return OS_EOK;
}

PUBLIC void HAL_DirectUartStage2(void)
{
    ASSERT(IRQ_DelayWorkInit(&UartWork, UartWorkHandler, NULL, IRQ_WORK_NOREENTER) == OS_EOK);
    ASSERT(IRQ_DelayQueueEnter(IRQ_NORMAL_QUEUE, &UartWork) == OS_EOK);

    ASSERT(IRQ_Bind(UART_IRQ, UartIrqHandler, NULL, "Uart", 0) == OS_EOK);
    ASSERT(IRQ_Unmask(UART_IRQ) == OS_EOK);
}
#else
PUBLIC void HAL_DirectUartStage2(void)
{
    /* do nothing */
}
#endif