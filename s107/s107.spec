// -*- C++ -*-

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

#include "spec/spec.spec"

#include "land_camac_converter.spec"

#include "land_fastbus.spec"

#include "land_type_subtype.spec"

EVENT
{
  // Globally will be added (always) (from event header):
  // TRIG, EVENTNO

  camac       = LAND_CAMAC_CONVERTER(type    = SUBEVENT_CAMAC,
				     subtype = SUBEVENT_CAMAC_CONVERTERS);
  fastbus             = LAND_FASTBUS(type    = SUBEVENT_FASTBUS,
				     subtype = SUBEVENT_FASTBUS_DATA1);
}

//#include "mapping_beam.hh"
#include "mapping_land.hh"
//#include "mapping_tdet.hh"

SIGNAL(ZERO_SUPPRESS: N1_1); // zero suppress along the second index
SIGNAL(ZERO_SUPPRESS: V1_1); // zero suppress along the second index
/*
SIGNAL(ZERO_SUPPRESS: TFW1); // zero suppress along the first index
SIGNAL(ZERO_SUPPRESS: TOF1); // zero suppress along the first index
SIGNAL(ZERO_SUPPRESS: CS1_1);
SIGNAL(ZERO_SUPPRESS: CV1_1);
SIGNAL(ZERO_SUPPRESS: GFI1_1);
SIGNAL(ZERO_SUPPRESS: FGR1_1);
*/
