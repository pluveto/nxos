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
#include <mods/thirdpart/cpio.h>
#include <utils/string.h>

#include <mm/alloc.h>

NX_IMPORT unsigned char __hex_file_rootfs[];

#define ROOT_PATH       "/"
#define ROOT_PATH_LEN   (sizeof(ROOT_PATH))

NX_INTERFACE void HAL_PlatformMain(void)
{
    NX_LOG_I("PC32 platform main running...\n");
    
    struct cpio_info info;
    cpio_info(__hex_file_rootfs, &info);
    char *path;
    int i;
    char* file_buf;
    char *filename;
    NX_USize file_sz;

    if ((path = (char *)NX_MemAlloc(info.max_path_sz + ROOT_PATH_LEN)) == NX_NULL)
    {
        NX_LOG_E("no memory (%dKB) enough", (info.max_path_sz + ROOT_PATH_LEN) / NX_KB);
        return;
    }

    for (i = 0; i < info.file_count; ++i)
    {
        file_buf = cpio_get_entry(__hex_file_rootfs, i, (const char**)&filename, &file_sz);
        NX_StrCopy(path, ROOT_PATH);
        NX_StrCopy(path + ROOT_PATH_LEN - 1, filename);
        
        if (file_sz == 0)
        {
            NX_LOG_I("file: %s dir", path);
        }
        else
        {
            NX_LOG_I("file: %s file", path);
            NX_LOG_I("file: %s", file_buf);
        }
    }

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
