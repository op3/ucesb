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

#ifndef __TCP_PIPE_BUFFER_HH__
#define __TCP_PIPE_BUFFER_HH__

#include "pipe_buffer.hh"

class lmd_input_tcp;

class tcp_pipe_buffer
  : public pipe_buffer_base
{
public:
  tcp_pipe_buffer();
  virtual ~tcp_pipe_buffer() { }

public:
  lmd_input_tcp *_server;

#ifdef USE_PTHREAD
public:
  virtual void *reader();
#else
public:
  virtual int read_now(off_t end);
#endif

public:
  void init(lmd_input_tcp *server,size_t bufsize
#ifdef USE_PTHREAD
	    ,thread_block *block_reader
#endif
	    );
  virtual void close();


};

#endif//__TCP_PIPE_BUFFER_HH__
