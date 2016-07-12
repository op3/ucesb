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

#ifndef __HAS_PTHREAD_GETNAME_NP_H__
#define __HAS_PTHREAD_GETNAME_NP_H__

#ifndef ACC_DEF_RUN
# include "gen/acc_auto_def/has_pthread_getname_np.h"
#endif

#if ACC_DEF_HAS_PTHREAD_GETNAME_NP_pthread_h
# define HAS_PTHREAD_GETNAME_NP 1
# ifndef _GNU_SOURCE
# define _GNU_SOURCE /* for pthread_setname_np */
# endif
#endif
#if ACC_DEF_HAS_PTHREAD_GETNAME_NP_notavail
# define HAS_PTHREAD_GETNAME_NP 0
#endif

#ifdef ACC_DEF_RUN
# include <pthread.h>

void acc_test_func(pthread_t thread)
{
#if HAS_PTHREAD_GETNAME_NP
  char fullname[16];
  pthread_getname_np(thread, fullname, sizeof (fullname));
  pthread_setname_np(thread, fullname);
#else
  (void) thread;
#endif
}
#endif

#endif/*__HAS_PTHREAD_GETNAME_NP_H__*/
