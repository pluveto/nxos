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
#include <PageZone.h>
#include <MM/Page.h>

#define NX_LOG_LEVEL NX_LOG_INFO
#define NX_LOG_NAME "Page"
#include <Utils/Log.h>

#include <XBook/Debug.h>

/**
 * Init physic memory and map kernel on virtual memory.
 */
NX_PUBLIC void HAL_PageZoneInit(void)
{    
    NX_USize memSize = DRAM_SIZE_DEFAULT;
    
    NX_LOG_I("Memory NX_USize: %x Bytes %d MB", memSize, memSize / NX_MB);

    if (memSize == 0)
    {
        NX_PANIC("Get Memory NX_USize Failed!");
    }
    if (memSize < MEM_MIN_SIZE)
    {
        NX_LOG_E("Must has %d MB memory!", MEM_MIN_SIZE / NX_MB);
        NX_PANIC("Memory too small");
    }
    
    /* calc normal base & size */
    NX_USize normalSize = memSize - MEM_KERNEL_SZ;
    normalSize /= 2;
    if (normalSize > MEM_KERNEL_SPACE_SZ)
    {
        normalSize = MEM_KERNEL_SPACE_SZ;
    }
    
    /* calc user base & size */
    NX_Addr userBase = MEM_NORMAL_BASE + normalSize;
    NX_USize userSize = memSize - normalSize;

    NX_LOG_I("Normal memory base: %x NX_USize:%d MB", MEM_NORMAL_BASE, normalSize / NX_MB);
    NX_LOG_I("User memory base: %x NX_USize:%d MB", userBase, userSize / NX_MB);

    /* init page zone */
    NX_PageInitZone(NX_PAGE_ZONE_NORMAL, (void *)MEM_NORMAL_BASE, normalSize);
    NX_PageInitZone(NX_PAGE_ZONE_USER, (void *)userBase, userSize);

    NX_LOG_I("Memroy init done.");
}

NX_IMPORT NX_Addr __NX_BssStart;
NX_IMPORT NX_Addr __NX_BssEnd;

NX_PUBLIC void HAL_ClearBSS(void)
{
    NX_MemZero(&__NX_BssStart, &__NX_BssEnd - &__NX_BssStart);
}
