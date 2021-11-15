/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Page test
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-16     JasonHu           Init
 */

#include <Mods/Test/Integration.h>
#include <MM/Page.h>
#define LOG_NAME "Page"
#include <Utils/Log.h>
#include <Utils/Memory.h>

#ifdef CONFIG_TEST_INTEGRATION_PAGE

INTEGRATION_TEST(PageAlloc)
{
    /* alloc memory */
    void *bufs[16];

    int i;
    for (i = 0; i < 16; i++)
    {
        void *buf = PageAlloc(i + 1);
        Zero(buf, PAGE_SIZE);
        LOG_D("Alloc: %x", buf);
        bufs[i] = buf; 
    }

    for (i = 0; i < 16; i++)
    {
        LOG_D("Free: %x", bufs[i]);
        if (bufs[i])
        {
            PageFree(bufs[i]);
        }
    }
    for (i = 0; i < 16; i++)
    {
        void *buf = PageAlloc(i + 1);
        Zero(buf, PAGE_SIZE);
        LOG_D("Alloc: %x", buf);
        bufs[i] = buf; 
    }

    for (i = 0; i < 16; i++)
    {
        void *buf = PageAllocInZone(PZ_DMA, i + 1);
        Zero(buf, PAGE_SIZE);
        LOG_D("Alloc: %x", buf);
        bufs[i] = buf; 
    }

    for (i = 0; i < 16; i++)
    {
        LOG_D("Free: %x", bufs[i]);
        if (bufs[i])
        {
            PageFreeInZone(PZ_DMA, bufs[i]);
        }
    }
    for (i = 0; i < 16; i++)
    {
        void *buf = PageAllocInZone(PZ_DMA, i + 1);
        Zero(buf, PAGE_SIZE);
        LOG_D("Alloc: %x", buf);
        bufs[i] = buf; 
    }
    return OS_EOK;
}

#endif
