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

#ifndef __HACKY_EXTERNAL_HH__
#define __HACKY_EXTERNAL_HH__

/*---------------------------------------------------------------------------*/

#include "external_data.hh"

/*---------------------------------------------------------------------------*/

class EXT_EBYE_DATA
{
public:
  EXT_EBYE_DATA();

public:
  int _group_item[0x100][0x40];
  int _group_data[0x100];
  int _ext_group_data[0x10000];

public:
  void __clean();
  EXT_DECL_UNPACK(/*_ARG:any arguments*/);

public:
  DUMMY_EXTERNAL_DUMP(EXT_EBYE_DATA);
  DUMMY_EXTERNAL_SHOW_MEMBERS(EXT_EBYE_DATA);
  DUMMY_EXTERNAL_ENUMERATE_MEMBERS(EXT_EBYE_DATA);
  DUMMY_EXTERNAL_ZERO_SUPPRESS_INFO_PTRS(EXT_EBYE_DATA);
};

DUMMY_EXTERNAL_MAP_STRUCT(EXT_EBYE_DATA);
DUMMY_EXTERNAL_WATCHER_STRUCT(EXT_EBYE_DATA);
DUMMY_EXTERNAL_CORRELATION_STRUCT(EXT_EBYE_DATA);

/*---------------------------------------------------------------------------*/

#endif//__CROS3_EXTERNAL_HH__
