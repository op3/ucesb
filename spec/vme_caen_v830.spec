// -*- C++ -*-

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

VME_CAEN_V830(geom)
{
  MEMBER(DATA32 data[32] ZERO_SUPPRESS);

  UINT32 header
    {
      0_15:  event_number;
      16_17: ts;
      18_23: count;
#ifdef VME_CAEN_V830_HEADER_24_25_UNDEFINED
      24_25: undefined;
#endif
      26:    1;
      27_31: geom = MATCH(geom);
    }

  list(0<=index<header.count)
    {
      UINT32 ch_data NOENCODE
	{
	  0_25:  value;
	  26:    0;
	  27_31: channel;

	  ENCODE(data[channel],(value=value));
	}
    }
}


