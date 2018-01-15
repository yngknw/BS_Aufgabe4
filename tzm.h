/*
 * tzm.h -- definitions for the char module
 *
 * Copyright (C) 2001 Alessandro Rubini and Jonathan Corbet
 * Copyright (C) 2001 O'Reilly & Associates
 *
 * The source code in this file can be freely used, adapted,
 * and redistributed in source or binary form, so long as an
 * acknowledgment appears in derived source files.  The citation
 * should list that the code comes from the book "Linux Device
 * Drivers" by Alessandro Rubini and Jonathan Corbet, published
 * by O'Reilly & Associates.   No warranty is attached;
 * we cannot take responsibility for errors or fitness for use.
 *
 * $Id: tzm.h,v 1.15 2004/11/04 17:51:18 rubini Exp $
 */

#ifndef _TZM_H_
#define _TZM_H_

#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */

#ifndef init_MUTEX
#define init_MUTEX(mutex) sema_init(mutex, 1)
#endif	/* init_MUTEX */

/*
 * Macros to help debugging
 */

#undef PDEBUG             /* undef it, just in case */
#ifdef TZM_DEBUG
#  ifdef __KERNEL__
     /* This one if debugging is on, and kernel space */
#    define PDEBUG(fmt, args...) printk( KERN_DEBUG "tzm: " fmt, ## args)
#  else
     /* This one for user space */
#    define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif
#else
#  define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

#undef PDEBUGG
#define PDEBUGG(fmt, args...) /* nothing: it's a placeholder */

#ifndef TZM_MAJOR_DYNAMIC
#define TZM_MAJOR_DYNAMIC 0   /* dynamic major by default */
#endif

#ifndef TZM_NR_DEVS
#define TZM_NR_DEVS 1    /* tzm0 through tzm3 */
#endif


/*
 * Split minors in two parts
 */
#define TYPE(minor)	(((minor) >> 4) & 0xf)	/* high nibble */
#define NUM(minor)	((minor) & 0xf)		/* low  nibble */

#endif /* _TZM_H_ */ 
