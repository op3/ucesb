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

#ifndef __THREAD_DEBUG_HH__
#define __THREAD_DEBUG_HH__

#include <sys/types.h>

#include <assert.h>

#define DEBUG_THREADING 0

#if DEBUG_THREADING

#ifdef  __ASSERT_FUNCTION
#define TDBG_FUNCTION __PRETTY_FUNCTION__ // __ASSERT_FUNCTION
#else
#define TDBG_FUNCTION __PRETTY_FUNCTION__ // __func__
#endif

#include <linux/unistd.h>
inline _syscall0(pid_t,gettid);

#define TDBG(...) { \
  fprintf(stderr,"<%5d> %s ",(int)gettid(),TDBG_FUNCTION); \
  fprintf(stderr,__VA_ARGS__); \
  fprintf(stderr,"\n"); \
}
#define TDBG_LINE(...) { \
  fprintf(stderr,__VA_ARGS__); \
  fprintf(stderr,"\n"); \
}

#else

#define TDBG(...) ((void) 0)
#define TDBG_LINE(...) ((void) 0)

#endif

#endif//__THREAD_DEBUG_HH__
