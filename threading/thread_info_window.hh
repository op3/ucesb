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

#ifndef __THREAD_INFO_WINDOW_HH__
#define __THREAD_INFO_WINDOW_HH__

#include "thread_info.hh"

#include <curses.h>

class thread_info_window
{
public:
  thread_info_window();

protected:
  WINDOW *mw;

  union
  {
    WINDOW *wall[5];
    struct
    {
      WINDOW *winput;
#ifdef USE_THREADING
      WINDOW *wtasks;
      WINDOW *wthreads;
#endif
      WINDOW *wtotals;
      WINDOW *werrors;
    };
  };

  thread_info *_ti;

public:
  void init(thread_info *ti);
  void display();

  void add_error(const char *text,
		 int severity);


};

#endif//__THREAD_INFO_WINDOW_HH__
