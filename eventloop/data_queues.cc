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

#include "data_queues.hh"


single_thread_queue<ofq_item,DATA_FILE_QUEUE_LEN> _open_file_queue;

fan_out_thread_queue<eq_item,UNPACK_QUEUE_LEN>    _unpack_event_queue;

fan_in_thread_queue<eq_item,RETIRE_QUEUE_LEN>     _retire_queue;


void processor_thread_data_queues::init(int index)
{
  _unpack = &_unpack_event_queue._queues[index];
  _retire =       &_retire_queue._queues[index];
}


#ifndef NDEBUG
void debug_queues_status()
{
  TDBG_LINE("Open file queue:");
  _open_file_queue.debug_status();
  TDBG_LINE("Unpack event queue:");
  _unpack_event_queue.debug_status();
  TDBG_LINE("Retire event queue:");
  _retire_queue.debug_status();
}
#endif
