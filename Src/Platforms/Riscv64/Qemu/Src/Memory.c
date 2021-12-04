/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Page init 
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-28     JasonHu           Init
 */

#include <Utils/Memory.h>

#include <Platform.h>
#include <Page.h>
#include <MM/Page.h>

#define LOG_LEVEL LOG_INFO
#define LOG_NAME "Page"
#include <Utils/Log.h>

#include <Utils/Debug.h>

/**
 * Init physic memory and map kernel on virtual memory.
 */
PUBLIC void PageInit(void)
{    
    Size memSize = DRAM_SIZE_DEFAULT;
    
    LOG_I("Memory Size: %x Bytes %d MB", memSize, memSize / SZ_MB);

    if (memSize == 0)
    {
        PANIC("Get Memory Size Failed!");
    }
    if (memSize < MEM_MIN_SIZE)
    {
        LOG_E("Must has %d MB memory!", MEM_MIN_SIZE / SZ_MB);
        PANIC("Memory too small");
    }
    
    /* calc normal base & size */
    Size normalSize = memSize - MEM_KERNEL_SZ;
    normalSize /= 2;
    if (normalSize > MEM_KERNEL_SPACE_SZ)
    {
        normalSize = MEM_KERNEL_SPACE_SZ;
    }
    
    /* calc user base & size */
    Addr userBase = MEM_NORMAL_BASE + normalSize;
    Size userSize = memSize - normalSize;

    LOG_I("Normal memory base: %x Size:%d MB", MEM_NORMAL_BASE, normalSize / SZ_MB);
    LOG_I("User memory base: %x Size:%d MB", userBase, userSize / SZ_MB);

    /* init page zone */
    PageInitZone(PZ_NORMAL, (void *)MEM_NORMAL_BASE, normalSize);
    PageInitZone(PZ_USER, (void *)userBase, userSize);

    LOG_I("Memroy init done.");
}

IMPORT Addr __OS_BssStart;
IMPORT Addr __OS_BssEnd;

PUBLIC void ClearBSS(void)
{
    Zero(&__OS_BssStart, &__OS_BssEnd - &__OS_BssStart);
}
