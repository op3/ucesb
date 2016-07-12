/* This file is part of UCESB - a tool for data unpacking and processing.
 *
 * Copyright (C) 2016  Haakan T. Johansson  <f96hajo@chalmers.se>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301  USA
 */

#ifndef __BYTEORDER_INCLUDE_H__
#define __BYTEORDER_INCLUDE_H__

#ifndef ACC_DEF_RUN
# include "gen/acc_auto_def/byteorder_include.h"
#endif

#if ACC_DEF_BYTEORDER_INCLUDE_byteorder_h
# include "byteorder.h"
#endif
#if ACC_DEF_BYTEORDER_INCLUDE_bsd_bsd_byteorder_h
# include "bsd/bsd_byteorder.h"
#endif
#if ACC_DEF_BYTEORDER_INCLUDE_endian_h
# include "endian.h"
#endif
#if ACC_DEF_BYTEORDER_INCLUDE_sys_endian_h
# include "sys/endian.h"
#endif
#if ACC_DEF_BYTEORDER_INCLUDE_machine_endian_h
# include "machine/endian.h"
#endif
#if 0 /* so far unused variants - kept for start to search... */
#if ACC_DEF_BYTEORDER_INCLUDE_machine_byte_order_h
# include "machine/byte_order.h"
#endif
#if ACC_DEF_BYTEORDER_INCLUDE_sys_types_h_stdint_h
/* OpenBSD ?? */
#include <sys/types.h>
#include <stdint.h>
#endif
#endif

#ifndef BYTE_ORDER
# if defined(__BYTE_ORDER)
#  define BYTE_ORDER     __BYTE_ORDER
#  define LITTLE_ENDIAN  __LITTLE_ENDIAN
#  define BIG_ENDIAN     __BIG_ENDIAN
# elif defined(_BYTE_ORDER)
#  define BYTE_ORDER     _BYTE_ORDER
#  define LITTLE_ENDIAN  _LITTLE_ENDIAN
#  define BIG_ENDIAN     _BIG_ENDIAN
# endif
#endif

#ifdef ACC_DEF_RUN
#ifndef BYTE_ORDER
# error BYTE_ORDER not defined
#endif
void acc_test_func()
{
}
#endif

#endif/*__BYTEORDER_INCLUDE_H__*/
