/**
 * Copyright (c) 2018-2022, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Platfrom main 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-18     JasonHu           Init
 */

#include <xbook.h>
#include <drivers/direct_uart.h>

#define NX_LOG_NAME "HAL_PlatformMain"
#include <utils/log.h>

#include <mods/fs/romfs.h>

unsigned long syscall(unsigned long api, 
    unsigned long a0,
    unsigned long a1,
    unsigned long a2,
    unsigned long a3,
    unsigned long a4,
    unsigned long a5,
    unsigned long a6)
{
    unsigned long ret;
    NX_CASM("movl %1, %%eax\n\t"
            "movl %2, %%ebx\n\t"
            "movl %3, %%ecx\n\t"
            "movl %4, %%edx\n\t"
            "movl %5, %%esi\n\t"
            "movl %6, %%edi\n\t"
            "movl %7, %%ebp\n\t"
            "int $0x80\n\t"
            "movl %%eax, %0\n\t"
            :"=m"(ret)
            :"m"(api), "m"(a0), "m"(a1), "m"(a2), "m"(a3), "m"(a4), "m"(a5), "m"(a6));
    return ret;
}

NX_INTERFACE void HAL_PlatformMain(void)
{
    NX_LOG_I("PC32 platform main running...\n");

    NX_RomfsMount("a", "b", 0);

    NX_LOG_I("call syscall\n");

    unsigned long ret = syscall(0, 0, 1,2,3,4,5,6);
    NX_LOG_I("call syscall %p\n", ret);
    
    ret = syscall(1, 0, 1,2,3,4,5,6);
    NX_LOG_I("call syscall %p\n", ret);
    
    NX_LOG_I("call syscall done\n");
}

NX_PUBLIC void HAL_DirectUartGetcHandler(char data)
{
    switch (data)
    {
    case '\r':
    case '\n':
        HAL_DirectUartPutc('\n');
        break;
    case '\b':
        HAL_DirectUartPutc('\b');
        HAL_DirectUartPutc(' ');
        HAL_DirectUartPutc('\b');
        break;
    default:
        HAL_DirectUartPutc(data);
        break;
    }
}
