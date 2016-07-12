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

// Definitions for EBYE format with 32 bit data entities

// Specification for EBYE data items, as used by the Daresbury MIDAS

#include "spec/midas32_caen.spec"

SIMPLE_DATA(group)
{
  MEMBER(DATA16 data[64] ZERO_SUPPRESS);

  UINT32 entry NOENCODE
    {
      0_15:  value;
      // 16_29: address;
      16_23: group = MATCH(group);
      24_29: item;
      30_31: 0b00;

      ENCODE(data[item],(value=value));
    }
}
