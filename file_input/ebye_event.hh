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

#ifndef __EBYE_EVENT_HH__
#define __EBYE_EVENT_HH__

#define EBYE_RECORD_ID "EBYEDATA"

struct ebye_record_header
{
  uint8  _id[8];       // EBYEDATA
  uint32 _sequence;
  uint16 _tape;        // 1
  uint16 _stream;      // 1..4
  uint16 _endian_tape; // native 1 by tape writer
  uint16 _endian_data; // native 1 by data writer
  uint32 _data_length; // in bytes, header not included
};

struct ebye_event_header
{
#ifdef USE_EBYE_INPUT_16
  uint16 _start_end_token;
  uint16 _length;
#endif
#ifdef USE_EBYE_INPUT_32
  uint32 _start_end_token_length;
#endif
};

#ifdef USE_EBYE_INPUT_16
typedef uint16 ebye_data_t;
#endif
#ifdef USE_EBYE_INPUT_32
typedef uint32 ebye_data_t;
#endif

#endif//__EBYE_EVENT_HH__
