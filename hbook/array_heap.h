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

#ifndef __ARRAY_HEAP_H__
#define __ARRAY_HEAP_H__

/* From LWROC, later DRASI...  */

/* The heap is stored in an @array, and has @entries items, each of
 * type @item_t.  They are ordered by the @compare_less function.
 */

#define HEAP_MOVE_DOWN(item_t, array, entries, compare_less,		\
		       move_index) do {					\
    int __parent = (move_index);					\
    for ( ; ; ) {							\
      int __child1 = __parent * 2 + 1;					\
      int __child2 = __child1 + 1;					\
      int __move_to = __parent;						\
      item_t __tmp_item;						\
      if (__child1 >= (entries))					\
	break; /* Parent has no children. */				\
      if (compare_less(array[__child1], array[__move_to]))		\
	__move_to = __child1;						\
      if (__child2 < (entries) &&					\
	  compare_less(array[__child2], array[__move_to]))		\
	__move_to = __child2;						\
      if (__move_to == __parent)					\
	break; /* Parent is smaller than children. */			\
      __tmp_item = array[__parent];					\
      array[__parent] = array[__move_to];				\
      array[__move_to] = __tmp_item;					\
      __parent = __move_to;						\
    }									\
  } while (0)

#define HEAP_INSERT(item_t, array, entries, compare_less,		\
		    insert_item) do {					\
    int __insert_at = (entries)++;					\
    while (__insert_at > 0) {						\
      int __parent_at = (__insert_at - 1) / 2;				\
      if (compare_less(array[__parent_at],insert_item))			\
	break; /* Parent is already smaller. */				\
      array[__insert_at] = array[__parent_at];				\
      __insert_at = __parent_at;					\
    }									\
    array[__insert_at] = insert_item;					\
  } while (0)

#endif/*__ARRAY_HEAP_H__*/
