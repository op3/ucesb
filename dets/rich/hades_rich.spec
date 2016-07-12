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

external EXT_HADES_RICH();

SUBEVENT(RICH_SUBEV)
{
  // It does not make much sense to subdivide all these into their own
  // data structures (one for each 64 channels)...  Use an external
  // unpacker.

  external data = EXT_HADES_RICH();

  /*
  UINT32 data NOENCODE
    {
      0_9:   value;
      10_15: channel;
      16_18: module;
      19_21: port;
      22:    controller;
      23_25: sector;
    };
  */
}

