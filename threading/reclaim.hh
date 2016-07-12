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

#ifndef __RECLAIM_HH__
#define __RECLAIM_HH__

#include "typedef.hh"

#define RECLAIM_THREAD_BUFFER_ITEM   0x0001
#define RECLAIM_MMAP_RELEASE_TO      0x0002 // implies RECLAIM_THREAD_BUFFER_ITEM
#define RECLAIM_PBUF_RELEASE_TO      0x0003 // implies RECLAIM_THREAD_BUFFER_ITEM
#define RECLAIM_MESSAGE              0x0004 // implies RECLAIM_THREAD_BUFFER_ITEM
#define RECLAIM_FORMAT_BUFFER_HEADER 0x0005 // implies RECLAIM_THREAD_BUFFER_ITEM

struct reclaim_item
{
  uint32        _type;
  reclaim_item *_next;
};

void reclaim_items(reclaim_item *first);
void print_format_reclaim_items(reclaim_item *first);

#endif//__RECLAIM_HH__
