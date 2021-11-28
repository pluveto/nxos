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
    OS_EOK = 0, /* ok */
    OS_ERROR,   /* error */
    OS_EPERM,   /* no perimisson */
    OS_EINVAL,  /* invalid arg */
    OS_ETIMEOUT,/* timeout */
    OS_EFAULT,  /* execute fault */
    OS_ENORES,  /* no resource */
    OS_EAGAIN,  /* try again later */
    OS_EINTR,   /* interrupt */
    OS_ENOMEM,  /* no memory */
    OS_ENOFUNC, /* no function */
    OS_ERROR_NR
};
typedef enum OS_Error OS_Error;

#endif  /* __XBOOK_ERROR__ */