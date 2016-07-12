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

#include "structures.hh"

#include "user.hh"

#include "multi_chunk_fcn.hh"

/*
uint32 AD413A_4CH::get_event_counter() const
{
  return 0;
}

uint32 AD413A_4CH::get_event_counter_offset(uint32 start) const
{
  return 0;
}
*/

int gamma_k8_user_function_multi(unpack_event *event)
{
#define MODULE event->ad413a.multi_adc

  for (unsigned int i = 0; i < MODULE._num_items; i++)
    {
      MODULE._item_event[i] = i;
    }

  MODULE.assign_events(MODULE._num_items);

  return MODULE._num_items;
}
