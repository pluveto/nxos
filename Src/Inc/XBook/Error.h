/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: Error number
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-10-16     JasonHu           Init
 */

#ifndef __XBOOK_ERROR__
#define __XBOOK_ERROR__

#include <XBook.h>

enum OS_Error
{
    OS_EOK = 0,
    OS_ERROR,
    OS_EPERM,
    OS_EINVAL,
    OS_ETIMEOUT,
    OS_EFAULT,
    OS_ENORES,
    OS_ERROR_NR
};
typedef enum OS_Error OS_Error;

#endif  /* __XBOOK_ERROR__ */