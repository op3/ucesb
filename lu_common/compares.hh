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

#ifndef __COMPARES_HH__
#define __COMPARES_HH__

template<typename T>
struct compare_ptr_less
{
  bool operator()(const T *s1,const T *s2) const
  {
    return *s1 < *s2;
  }
};

template<class T>
struct compare_ptrs_strcmp
{
  bool operator()(const T& s1,const T& s2) const
  {
    return strcmp(s1,s2) < 0;
  }
};

typedef compare_ptrs_strcmp<const char *> compare_str_less;
/*
struct compare_str_less
{
  bool operator()(const char *s1,const char *s2) const
  {
    return strcmp(s1,s2) < 0;
  }
};
*/

#endif/*__COMPARES_HH__*/
