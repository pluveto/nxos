/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 * SPDX-License-Identifier: Apache-2.0
 *
 * Contains: Direct uart driver
 * 
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-1     JasonHu       first version
 */

#include <XBook.h>
#include <HAL.h>
#include <I386.h>
#include <DirectUart.h>

#define UART_SEND_TIMEOUT       1

/* I/O port base addr */
#define COM1_BASE               0X3F8
#define BAUD_VALUE_MAX          115200
#define BAUD_VALUE_DEFAULT      115200
#define BAUD_DIVISOR_DEFAULT    (BAUD_VALUE_MAX / BAUD_VALUE_DEFAULT)

enum UartFifoControl
{
    FIFO_ENABLE = 1,                             /* Enable FIFOs */
    FIFO_CLEAR_RECEIVE   = (1 << 1),             /* Clear Receive FIFO */
    FIFO_CLEAR_TRANSMIT  = (1 << 2),             /* Clear Transmit FIFO */
    FIFO_DMA_MODE_SELECT = (1 << 3),             /* DMA Mode Select */
    FIFO_RESERVED        = (1 << 4),             /* Reserved */
    FIFO_ENABLE_64       = (1 << 5),             /* Enable 64 Byte FIFO(16750) */
    /* Interrupt Trigger Level/Trigger Level  */
    FIFO_TRIGGER_1       = (0 << 6),             /* 1 Byte */
    FIFO_TRIGGER_4       = (1 << 6),             /* 4 Byte */
    FIFO_TRIGGER_8       = (1 << 7),             /* 8 Byte */
    FIFO_TRIGGER_14      = (1 << 6) | (1 << 7),  /* 14 Byte */
};

enum UartLineControl
{
    /* Word Length */
    LINE_WORD_LENGTH_5   = 0,                    /* 5 Bits */
    LINE_WORD_LENGTH_6   = 1,                    /* 6 Bits */
    LINE_WORD_LENGTH_7   = (1 << 1),             /* 7 Bits */
    LINE_WORD_LENGTH_8   = ((1 << 1) | 1),       /* 8 Bits */
    LINE_STOP_BIT_1      = (0 << 2),             /* One Stop Bit */
    LINE_STOP_BIT_2      = (1 << 2),             /* 1.5 Stop Bits or 2 Stop Bits */
    /* Parity Select */
    LINE_PARITY_NO       = (0 << 3),             /* No Parity */
    LINE_PARITY_ODD      = (1 << 3),             /* Odd Parity */
    LINE_PARITY_EVEN     = (1 << 3) | (1 << 4),  /* Even Parity */
    LINE_PARITY_MARK     = (1 << 3) | (1 << 5),  /* Mark */
    LINE_PARITY_SPACE    = (1 << 3) | (1 << 4) | (1 << 5), /* Space */
    LINE_BREAK_ENABLE    = (1 << 6),             /* Set Break Enable */
    LINE_DLAB            = (1 << 7),             /* Divisor Latch Access Bit */
};

enum UartInterruptEnable
{
    INTR_RECV_DATA_AVALIABLE = 1,        /* Enable Received Data Available Interrupt */
    INTR_TRANSMIT_HOLDING    = (1 << 1), /* Enable Transmitter Holding Register Empty Interrupt */
    INTR_STATUS_CHANGED      = (1 << 2), /* Enable Receiver Line Status Interrupt */
    INTR_MODEM_STATUS        = (1 << 3), /* Enable Modem Status Interrupt */
    INTR_SLEEP_MODE          = (1 << 4), /* Enable Sleep Mode(16750) */
    INTR_LOW_POWER_MODE      = (1 << 5), /* Enable Low Power Mode(16750) */
    INTR_RESERVED1           = (1 << 6), /* Reserved */
    INTR_RESERVED2           = (1 << 7), /* Reserved */
};

enum UartLineStatus
{
    LINE_STATUS_DATA_READY                  = 1,        /* Data Ready */
    LINE_STATUS_OVERRUN_ERROR               = (1 << 1), /* Overrun Error */
    LINE_STATUS_PARITY_ERROR                = (1 << 2), /* Parity Error */
    LINE_STATUS_FRAMING_ERROR               = (1 << 3), /* Framing Error */
    LINE_STATUS_BREAK_INTERRUPT             = (1 << 4), /* Break Interrupt */
    LINE_STATUS_EMPTY_TRANSMITTER_HOLDING   = (1 << 5), /* Empty Transmitter Holding Register */
    LINE_STATUS_EMPTY_DATA_HOLDING          = (1 << 6), /* Empty Data Holding Registers */
    LINE_STATUS_ERROR_RECEIVE_FIFO          = (1 << 7), /* Error in Received FIFO */
};

enum UartIntrIndenty
{
    INTR_STATUS_PENDING_FLAG        = 1,        /* Interrupt Pending Flag */
    INTR_STATUS_MODEM               = (0 << 1), /* Transmitter Holding Register Empty Interrupt  */
    INTR_STATUS_TRANSMITTER_HOLDING = (1 << 1), /* Received Data Available Interrupt */
    INTR_STATUS_RECEIVE_DATA        = (1 << 2), /* Received Data Available Interrupt */
    INTR_STATUS_RECEIVE_LINE        = (1 << 1) | (1 << 2),  /* Receiver Line Status Interrupt */
    INTR_STATUS_TIME_OUT_PENDING    = (1 << 2) | (1 << 3),  /* Time-out Interrupt Pending (16550 & later) */
    INTR_STATUS_64BYTE_FIFO         = (1 << 5), /* 64 Byte FIFO Enabled (16750 only) */
    INTR_STATUS_NO_FIFO             = (0 << 6), /* No FIFO on chip */
    INTR_STATUS_RESERVED_CONDITION  = (1 << 6), /* Reserved condition */
    INTR_STATUS_FIFO_NOT_FUNC       = (1 << 7), /* FIFO enabled, but not functioning */
    INTR_STATUS_FIFO                = (1 << 6) | (1 << 7),  /* FIFO enabled */
};

struct DirectUart
{
    U16 iobase;
    U16 data;
    U16 divisorLow;
    U16 intrEnable;
    U16 divisorHigh;
    U16 intrIndenty;
    U16 fifo;
    U16 lineCtrl;
    U16 modemCtrl;
    U16 lineStatus;
    U16 modem_status;
    U16 scratch;
};

PRIVATE struct DirectUart directUart;

PRIVATE void UartSent(struct DirectUart *uart, char data)
{
#if UART_SEND_TIMEOUT == 1
    int timeout = 0x100000;
    while (!(IO_In8(uart->lineStatus) & LINE_STATUS_EMPTY_TRANSMITTER_HOLDING) && timeout--)
    {
    }
#else
    while (!(IO_In8(uart->lineStatus) & LINE_STATUS_EMPTY_TRANSMITTER_HOLDING))
    {
    }
#endif
    IO_Out8(uart->data, data);
}

PUBLIC void HAL_DirectUartPutc(char ch)
{
    if(ch == '\n') {
        UartSent(&directUart, '\r');
    }
    UartSent(&directUart, ch);
}

INTERFACE void HAL_ConsoleOutChar(char ch)
{
    HAL_DirectUartPutc(ch);
}

PUBLIC void HAL_DirectUartInit(void)
{
    struct DirectUart *uart = &directUart;
    U16 iobase = COM1_BASE;

    uart->iobase                    = iobase;
    uart->data                      = iobase + 0;
    uart->divisorLow                = iobase + 0;
    uart->intrEnable                = iobase + 1;
    uart->divisorHigh               = iobase + 1;
    uart->intrIndenty               = iobase + 2;
    uart->lineCtrl                  = iobase + 3;
    uart->modemCtrl                 = iobase + 4;
    uart->lineStatus                = iobase + 5;
    uart->modem_status              = iobase + 6;
    uart->scratch                   = iobase + 7;

    IO_Out8(uart->lineCtrl, LINE_DLAB);

    IO_Out8(uart->divisorLow, (BAUD_DIVISOR_DEFAULT) & 0xff);
    IO_Out8(uart->divisorHigh, ((BAUD_DIVISOR_DEFAULT) >> 8) & 0xff);

    IO_Out8(uart->lineCtrl, LINE_WORD_LENGTH_8 |
        LINE_STOP_BIT_1 | LINE_PARITY_NO);

    /* close all intr */
    IO_Out8(uart->intrEnable, 0);

    IO_Out8(uart->fifo, FIFO_ENABLE | FIFO_CLEAR_TRANSMIT |
        FIFO_CLEAR_RECEIVE | FIFO_ENABLE_64 |
        FIFO_TRIGGER_14);

    IO_Out8(uart->modemCtrl, 0x00);
    IO_Out8(uart->scratch, 0x00);
}
