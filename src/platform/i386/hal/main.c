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

NX_INTERFACE void HAL_PlatformMain(void)
{
    NX_LOG_I("PC32 platform main running...\n");

    NX_Error err;
    NX_RomfsFile *file;
    char buf[32] = {0};
    NX_USize rd;
    NX_Offset off;

    NX_RomfsMount("/", "dev", 0);

    err = NX_RomfsOpen("dummy.txt", 0, &file);
    if (err != NX_EOK)
    {
        NX_LOG_E("open file failed with %d", err);
        return;
    }

    err = NX_RomfsRead(file, buf, 32, &rd);
    if (err != NX_EOK)
    {
        NX_LOG_E("read file failed with %d", err);
        return;
    }
    NX_LOG_I("read file bytes %d", rd);
    
    NX_LOG_I("file: %s", buf);
    
    err = NX_RomfsRead(file, buf, 32, &rd);
    if (err != NX_EOK)
    {
        NX_LOG_E("read file failed with %d, as expect", err);
    }
    NX_LOG_I("read file bytes %d", rd);
    
    err = NX_RomfsSeek(file, 0, NX_ROMFS_SEEK_SET, &off);
    if (err != NX_EOK)
    {
        NX_LOG_E("seek file failed with %d", err);
        return;
    }
    NX_LOG_I("seek file off %d", off);

    err = NX_RomfsRead(file, buf, 32, &rd);
    if (err != NX_EOK)
    {
        NX_LOG_E("read file failed with %d", err);
        return;
    }
    NX_LOG_I("read file bytes %d", rd);
    
    err = NX_RomfsRead(file, buf, 32, &rd);
    if (err != NX_EOK)
    {
        NX_LOG_E("read file failed with %d, as expect", err);
    }
    NX_LOG_I("read file bytes %d", rd);
    
    err = NX_RomfsSeek(file, 0, NX_ROMFS_SEEK_SET, &off);
    if (err != NX_EOK)
    {
        NX_LOG_E("seek file failed with %d", err);
        return;
    }
    NX_LOG_I("seek file off %d", off);

    err = NX_RomfsRead(file, buf, 32, &rd);
    if (err != NX_EOK)
    {
        NX_LOG_E("read file failed with %d", err);
        return;
    }
    NX_LOG_I("read file bytes %d", rd);
    
    err = NX_RomfsSeek(file, -rd, NX_ROMFS_SEEK_CUR, &off);
    if (err != NX_EOK)
    {
        NX_LOG_E("seek file failed with %d", err);
        return;
    }
    NX_LOG_I("seek file off %d", off);

    err = NX_RomfsRead(file, buf, 32, &rd);
    if (err != NX_EOK)
    {
        NX_LOG_E("read file failed with %d", err);
        return;
    }
    NX_LOG_I("read file bytes %d", rd);
    
    err = NX_RomfsRead(file, buf, 32, &rd);
    if (err != NX_EOK)
    {
        NX_LOG_E("read file failed with %d, as expect", err);
    }
    NX_LOG_I("read file bytes %d", rd);
    
    err = NX_RomfsSeek(file, 0, NX_ROMFS_SEEK_SET, &off);
    if (err != NX_EOK)
    {
        NX_LOG_E("seek file failed with %d", err);
        return;
    }
    NX_LOG_I("seek file off %d", off);

    err = NX_RomfsRead(file, buf, 32, &rd);
    if (err != NX_EOK)
    {
        NX_LOG_E("read file failed with %d", err);
        return;
    }
    NX_LOG_I("read file bytes %d", rd);
    
    err = NX_RomfsSeek(file, -rd, NX_ROMFS_SEEK_END, &off);
    if (err != NX_EOK)
    {
        NX_LOG_E("seek file failed with %d", err);
        return;
    }
    NX_LOG_I("seek file off %d", off);

    err = NX_RomfsRead(file, buf, 32, &rd);
    if (err != NX_EOK)
    {
        NX_LOG_E("read file failed with %d", err);
        return;
    }
    NX_LOG_I("read file bytes %d", rd);
    
    err = NX_RomfsRead(file, buf, 32, &rd);
    if (err != NX_EOK)
    {
        NX_LOG_E("read file failed with %d, as expect", err);
    }
    NX_LOG_I("read file bytes %d", rd);
    
    err = NX_RomfsClose(file);
    if (err != NX_EOK)
    {
        NX_LOG_E("close file failed with %d", err);
    }

    NX_LOG_I("test done");
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
