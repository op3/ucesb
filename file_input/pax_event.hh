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

#ifndef __PAX_EVENT_HH__
#define __PAX_EVENT_HH__

#define PAX_RECORD_MAX_LENGTH     32764

#define PAX_RECORD_TYPE_UNDEF     0
#define PAX_RECORD_TYPE_STD       0x0001
#define PAX_RECORD_TYPE_STD_SWAP  0x0100

struct pax_record_header
{
  uint16 _length;
  uint16 _type;
  uint16 _seqno;
  uint16 _off_nonfrag;
};

#define PAX_EVENT_TYPE_NORMAL_MAX  31

struct pax_event_header
{
  uint16 _length;
  uint16 _type;
};

#endif//__PAX_EVENT_HH__
