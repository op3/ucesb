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

#ifndef __MC_DEF_HH__
#define __MC_DEF_HH__

#include <vector>

#include "../common/signal_id.hh"
#include "../common/node.hh"

extern def_node_list *all_mc_defs;

#include "map_info.hh"
#include "calib_info.hh"

char *argv0_replace(const char *filename);

void read_map_calib_info();

void process_map_calib_info();

const signal_id_info *get_signal_id_info(signal_id *id,
					 int map_no);

void verify_units_match(const prefix_units_exponent *dest,
			const units_exponent *src,
			const file_line &loc,
			const char *descr,double &factor);

#endif//__MC_DEF_HH__
