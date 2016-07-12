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

#ifndef __EXT_DATA_CLNT_HH__
#define __EXT_DATA_CLNT_HH__

#ifndef __CINT__
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#endif

class ext_data_struct_info;

class ext_data_clnt
{
public:
  ext_data_clnt();
  ~ext_data_clnt();

public:
  bool connect(const char *server);
  bool connect(const char *server,int port);
  bool connect(int fd);

  int setup(const void *struct_layout_info,size_t size_info,
	    ext_data_struct_info *struct_info,
	    size_t size_buf);

  int fetch_event(void *buf,size_t size);
  int get_raw_data(const void **raw, ssize_t *raw_words);
  const char *last_error();

  int close();

  static void rand_fill(void *buf,size_t size);

protected:
  void *_client; // void such that cint does not care about the type

};

#endif//__EXT_DATA_CLNT_HH__
