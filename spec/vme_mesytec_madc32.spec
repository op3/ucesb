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

VME_MESYTEC_MADC32(geom)
{
  MEMBER(DATA12_OVERFLOW data[32] ZERO_SUPPRESS);

  UINT32 header NOENCODE
  {
    0_11:  word_number; // includes end_of_event
    12_14: adc_resol;
    15:	   out_form;
    16_23: geom = MATCH(geom);
    24_29: 0b000000;
    30_31: 0b01;
  }

  list(0 <= index < static_cast<uint32>(header.word_number - 1))
  {
    UINT32 ch_data NOENCODE
    {
      0_11:  value;
      14:    outofrange;
      16_20: channel;
      21_29: 0b000100000;
      30_31: 0b00;

      ENCODE(data[channel], (value = value, overflow = outofrange));
    }
  }

  UINT32 end_of_event NOENCODE
  {
    0_29:  counter;
    30_31: 0b11;
  }
}
