/* This file is part of UCESB - a tool for data unpacking and processing.
 *
 * Copyright (C) 2016  GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
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

#ifndef __CALIB_PARAM_HH__
#define __CALIB_PARAM_HH__

#include "struct_calib.hh"

#include "event_base.hh"

//#define CALIB_SLOPE_OFFSET(slope,offset) new_slope_offset(/*__src_ptr*/&(_event._raw.channel),(slope),(offset))
//#define CALIB_OFFSET_SLOPE(offset,slope) new_offset_slope(__src_ptr,(offset),(slope))

#define NEW_SLOPE_OFFSET  new_slope_offset
#define NEW_OFFSET_SLOPE  new_offset_slope

//#define NAME_SLOPE_OFFSET(slope,offset)  _slope_offset
//#define NAME_OFFSET_SLOPE(slope,offset)  _offset_slope

#define CALIB_PARAM(channel,type,...) { \
  the_raw_event_calib_map.channel._calib = NEW_##type(&(_event._raw.channel), \
                                                      &(_event._cal.channel), \
                                                      __VA_ARGS__); \
}

#endif//__CALIB_PARAM_HH__
