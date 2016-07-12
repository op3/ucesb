/* This file is part of UCESB - a tool for data unpacking and processing.
 *
 * Copyright (C) 2016  GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
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

#ifndef __CONTROL_INCLUDE_HH__
#define __CONTROL_INCLUDE_HH__

#ifdef CONTROL_INCLUDE
#include "control.hh"
#endif

#ifndef USING_MULTI_EVENTS
#define USING_MULTI_EVENTS 0
#endif

#if USING_MULTI_EVENTS
#ifndef UNPACK_EVENT_USER_FUNCTION
#error UNPACK_EVENT_USER_FUNCTION must be defined when USING_MULTI_EVENTS
#endif
#endif

/* Warn if there are some troubles with old bad names. */

#ifdef INITUSERFUNCTION
#error INITUSERFUNCTION deprecated, use INIT_USER_FUNCTION
#endif

#ifdef EXITUSERFUNCTION
#error EXITUSERFUNCTION deprecated, use EXIT_USER_FUNCTION
#endif

#ifdef RAWEVENTUSERFUNCTION
#error RAWEVENTUSERFUNCTION deprecated, use RAW_EVENT_USER_FUNCTION
#endif

#ifdef CALEVENTUSERFUNCTION
#error CALEVENTUSERFUNCTION deprecated, use CAL_EVENT_USER_FUNCTION
#endif

#endif//__CONTROL_INCLUDE_HH__
