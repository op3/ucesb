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

#ifndef __CONTROL_HH__
#define __CONTROL_HH__

#define INIT_USER_FUNCTION          fa192mar09_user_init
#define EXIT_USER_FUNCTION          fa192mar09_user_exit

#define UNPACK_EVENT_USER_FUNCTION  fa192mar09_user_function_multi
#define RAW_EVENT_USER_FUNCTION     fa192mar09_user_function_raw
#define CAL_EVENT_USER_FUNCTION     fa192mar09_user_function

#define HANDLE_COMMAND_LINE_OPTION  fa192mar09_handle_command_line_option
#define USAGE_COMMAND_LINE_OPTIONS  fa192mar09_usage_command_line_options

#define USER_EXTERNAL_UNPACK_STRUCT_FILE "fa192mar09_external.hh"

#define WATCHER_EVENT_INFO_USER_FUNCTION fa192mar09_watcher_event_info

#define WATCHER_EVENT_INFO_INCLUDE "fa192mar09_watch_info.hh"

#define USING_MULTI_EVENTS 1

#endif//__CONTROL_HH__
