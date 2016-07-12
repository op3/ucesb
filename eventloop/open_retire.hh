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

#ifndef __OPEN_RETIRE_HH__
#define __OPEN_RETIRE_HH__

#include "worker_thread.hh"
#include "config.hh"
#include "decompress.hh"

class open_retire :
  public worker_thread
{
public:
  open_retire();
  virtual ~open_retire();

public:
  config_input_vect::iterator _input_iter;


public:
  void init();


public:
  bool open_file(int wakeup_this_file,
		 thread_block *block_reader);
  void close_file(data_input_source *source);
  void send_flush(int info);

public:
  virtual void *worker();



};

#endif//__OPEN_RETIRE_HH__
