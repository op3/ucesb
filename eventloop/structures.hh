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

#ifndef __STRUCTURES_HH__
#define __STRUCTURES_HH__

#include "control_include.hh"

#include "endian.hh"
#include "typedef.hh"

#include "data_src.hh"

#include "multi_chunk.hh"

#include "raw_data.hh"

#include "zero_suppress.hh"

#include "enumerate.hh"

#include "event_struct.hh"

#ifdef USER_EXTERNAL_UNPACK_STRUCT_FILE
#include USER_EXTERNAL_UNPACK_STRUCT_FILE
#endif

struct subevent_header; // forward, we do not want to depend on eventapi here!

#include "struct_fcns.hh"

// Forward declarations
/*
#define STRUCT_MIRROR_TEMPLATE template<typename T_watcher_channel>
#define STRUCT_MIRROR_STRUCT(type)         type##_watcher
#include "gen/struct_mirror_decl.hh"
#undef STRUCT_MIRROR_TEMPLATE
#undef STRUCT_MIRROR_STRUCT
*/
#include "gen/structures.hh"

#include "gen/raw_structure.hh"

#include "gen/cal_structure.hh"

#ifdef USER_STRUCT_FILE
#include USER_STRUCT_FILE
#endif

#ifdef CALIB_STRUCT_FILE
#include CALIB_STRUCT_FILE
#endif

#endif//__STRUCTURES_HH__

