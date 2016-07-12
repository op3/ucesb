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

#ifndef __SST_EXT_HH__
#define __SST_EXT_HH__

#include "data_src.hh"
#include "dummy_external.hh"
#include "external_data.hh"
#include "zero_suppress.hh"
#include "raw_data.hh"

/*---------------------------------------------------------------------------*/

DUMMY_EXTERNAL_MAP_STRUCT_FORW(EXT_SST);

union EXT_SST_header
{
  struct
  {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint32 count : 12; // 0..11
    uint32 local_event_counter : 4; // 12..15
    uint32 local_trigger : 4; // 16..19
    uint32 siderem : 4; // 20..23
    uint32 gtb : 4; // 24..27
    uint32 sam : 4; // 28..31
#endif
#if __BYTE_ORDER == __BIG_ENDIAN
    uint32 sam : 4; // 28..31
    uint32 gtb : 4; // 24..27
    uint32 siderem : 4; // 20..23
    uint32 local_trigger : 4; // 16..19
    uint32 local_event_counter : 4; // 12..15
    uint32 count : 12; // 0..11
#endif
  };
  uint32  u32;
};

class EXT_SST
{
public:
  EXT_SST();
  ~EXT_SST();

public:
  EXT_SST_header header;
  //raw_array_zero_suppress<DATA12,DATA12,512> data[4];

  raw_array_zero_suppress<DATA12,DATA12,1024> data;

public:
  void __clean();
  EXT_DECL_UNPACK_ARG(uint32 sam,uint32 gtb,uint32 siderem);
  EXT_DECL_UNPACK_ARG(uint32 sam,uint32 gtb,uint32 siderem,uint32 branch);
  // Needed if it is part of a select statement
  EXT_DECL_MATCH_ARG(uint32 sam,uint32 gtb,uint32 siderem);
  EXT_DECL_MATCH_ARG(uint32 sam,uint32 gtb,uint32 siderem,uint32 branch);


public:
  DUMMY_EXTERNAL_DUMP(EXT_SST);
  DUMMY_EXTERNAL_SHOW_MEMBERS(EXT_SST);
  DUMMY_EXTERNAL_ENUMERATE_MEMBERS(EXT_SST);
  DUMMY_EXTERNAL_ZERO_SUPPRESS_INFO_PTRS(EXT_SST);
};

DUMMY_EXTERNAL_MAP_STRUCT(EXT_SST);
DUMMY_EXTERNAL_WATCHER_STRUCT(EXT_SST);
DUMMY_EXTERNAL_CORRELATION_STRUCT(EXT_SST);

/*---------------------------------------------------------------------------*/

#endif//__SST_EXT_HH__
