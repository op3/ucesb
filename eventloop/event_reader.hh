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

#ifndef __EVENT_READER_HH__
#define __EVENT_READER_HH__

#include "worker_thread.hh"

#include "lmd_input.hh"
#include "pax_input.hh"
#include "genf_input.hh"
#include "ebye_input.hh"
#include "ridf_input.hh"

class data_input_source;

class event_reader :
  public worker_thread
{
public:
  virtual ~event_reader();

public:
  // The record/event reader
#ifdef USE_LMD_INPUT
  lmd_source _reader;
#endif
#ifdef USE_PAX_INPUT
  pax_source _reader;
#endif
#ifdef USE_GENF_INPUT
  genf_source _reader;
#endif
#ifdef USE_EBYE_INPUT
  ebye_source _reader;
#endif
#ifdef USE_RIDF_INPUT
  ridf_source _reader;
#endif

public:
  virtual void *worker();

public:
  void process_file(data_input_source *source);

  void wait_for_unpack_queue_slot();


};

#endif//__EVENT_READER_HH__
