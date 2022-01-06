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

enum NX_Error
{
    NX_EOK = 0, /* ok */
    NX_ERROR,   /* error */
    NX_EPERM,   /* no perimisson */
    NX_EINVAL,  /* invalid arg */
    NX_ETIMEOUT,/* timeout */
    NX_EFAULT,  /* execute fault */
    NX_ENORES,  /* no resource */
    NX_EAGAIN,  /* try again later */
    NX_EINTR,   /* interrupt */
    NX_ENOMEM,  /* no memory */
    NX_ENOFUNC, /* no function */
    NX_ENOSRCH, /* no search/found */
    NX_EIO,     /* mmio/portio */
    NX_ERROR_NR
};
typedef enum NX_Error NX_Error;

#endif  /* __XBOOK_ERROR__ */