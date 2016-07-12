/* This file is part of UCESB - a tool for data unpacking and processing.
 *
 * Copyright (C) 2016  Hans Toernqvist  <h.toernqvist@gsi.de>
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

#ifndef __RIDF_EVENT_HH__
#define __RIDF_EVENT_HH__

struct ridf_header
{
  uint32 _code;
  uint32 _address;
};

struct ridf_event_header
{
  struct ridf_header _header;
  uint32 _number;
};

struct ridf_subevent_header
{
  struct ridf_header _header;
  uint32 _id;
};

#endif // __RIDF_EVENT_HH__
