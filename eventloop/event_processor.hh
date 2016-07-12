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

#ifndef __EVENT_PROCESSOR_HH__
#define __EVENT_PROCESSOR_HH__

#include "worker_thread.hh"
#include "data_queues.hh"
#include "event_loop.hh"

class event_processor :
  public worker_thread
{
public:
  virtual ~event_processor();




public:
  processor_thread_data_queues _queues;

public:
#if defined(USE_LMD_INPUT) || defined(USE_HLD_INPUT) || defined(USE_RIDF_INPUT)
  ucesb_event_loop::source_event_hint_t _hints;
#endif

public:
  virtual void *worker();

public:
  void wait_for_output_queue_slot();
  void wait_for_input_queue_item();

public:
  void init(int index);

};

#endif//__EVENT_PROCESSOR_HH__
