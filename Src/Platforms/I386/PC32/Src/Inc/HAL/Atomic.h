/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: HAL Atomic
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-11-11     JasonHu           Init
 */

#ifndef __HAL_ATOMIC__
#define __HAL_ATOMIC__

#include <XBook.h>

struct HAL_Atomic
{
    VOLATILE int value;
};
typedef struct HAL_Atomic HAL_Atomic;

#endif /* __HAL_ATOMIC__ */
