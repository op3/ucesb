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

ITEM_LIMITED_MIDAS_CAEN_V785(group,minitem=0,maxitem=31)
{
  MEMBER(DATA12 data[32] ZERO_SUPPRESS);

  UINT32 entry NOENCODE
    {
      0_11:  value;
      12_15: 0;
      // 16_29: address;
      16_23: group = MATCH(group);
      24_28: item = RANGE(minitem,maxitem);
      29_31: 0b00;

      ENCODE(data[item],(value=value));
    }
}


ITEM_LIMITED_MIDAS_CAEN_V830(group,minitem=0,maxitem=31)
{
  MEMBER(DATA32 data[32] ZERO_SUPPRESS);

  UINT32 entry1 NOENCODE
    {
      0_15:  value;
      // 16_29: address;
      16_23: group = MATCH(group);
      24:    item_low = 0;
      25_29: item = RANGE(minitem,maxitem);
      30_31: 0b00;
    }

  UINT32 entry2 NOENCODE
    {
      0_15:  value;
      // 16_29: address;
      16_23: group = MATCH(group);
      24:    item_low = 1;
      // With |1, it becomes necessary for it to be one above (or the same (bad))
      // With +1, it becomes necessary for it to be the next (can start odd (bad))
      25_29: item = CHECK(entry1.item);
      30_31: 0b00;
    }

  ENCODE(data[entry1.item],(value=(entry1.value) | (entry2.value << 16)));
}








