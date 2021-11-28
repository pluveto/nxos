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

#include <MMU.h>
#include <Page.h>
#include <Platform.h>

#include <Utils/Memory.h>

#define LOG_LEVEL LOG_INFO
#define LOG_NAME "Page"
#include <Utils/Log.h>

#include <Utils/Debug.h>

#define GRUB2_READ_MEMORY_BYTES_ADDR (0x000001000)

PUBLIC MMU kernelMMU;

PRIVATE VOLATILE U32 kernelTable[PAGE_SIZE / sizeof(U32)] CALIGN(PAGE_SIZE);

/**
 * Init physic memory and map kernel on virtual memory.
 */
PUBLIC void PageInit(void)
{    
    U32 memSize = *(U32 *)GRUB2_READ_MEMORY_BYTES_ADDR;
    
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
    U32 normalSize = memSize - MEM_DMA_SIZE - MEM_KERNEL_SZ;
    normalSize /= 2;
    if (normalSize + MEM_NORMAL_BASE > MEM_KERNEL_TOP)
    {
        normalSize = MEM_KERNEL_TOP - MEM_NORMAL_BASE;
    }
    
    /* calc user base & size */
    Addr userBase = MEM_NORMAL_BASE + normalSize;
    Size userSize = memSize - userBase;

    LOG_I("DMA memory base: %x Size:%d MB", MEM_DMA_BASE, MEM_DMA_SIZE / SZ_MB);
    LOG_I("Normal memory base: %x Size:%d MB", MEM_NORMAL_BASE, normalSize / SZ_MB);
    LOG_I("User memory base: %x Size:%d MB", userBase, userSize / SZ_MB);

    /* init page zone */
    PageInitZone(PZ_DMA, (void *)MEM_DMA_BASE, MEM_DMA_SIZE);
    PageInitZone(PZ_NORMAL, (void *)MEM_NORMAL_BASE, normalSize);
    PageInitZone(PZ_USER, (void *)userBase, userSize);

    kernelMMU.virStart = 0;
    kernelMMU.earlyEnd = userBase;
    kernelMMU.virEnd = MEM_KERNEL_TOP;
    kernelMMU.table = kernelTable;

    MMU_EarlyMap(&kernelMMU, kernelMMU.virStart, kernelMMU.earlyEnd);

    MMU_SetPageTable((Uint)kernelMMU.table);
    MMU_Enable();

    LOG_I("MMU enabled");
}

IMPORT Addr __OS_BssStart;
IMPORT Addr __OS_BssEnd;

PUBLIC void ClearBSS(void)
{
    Zero(&__OS_BssStart, &__OS_BssEnd - &__OS_BssStart);
}
