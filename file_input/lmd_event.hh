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

#ifndef __LMD_EVENT_H__
#define __LMD_EVENT_H__

#include "lmd_file_header.h"
#include "lmd_buf_header.h"

#include "lmd_event_10_1.h"
#include "lmd_subevent_10_1.h"

#include <stdlib.h>

// convert from nutty specification to full buffer size in bytes
// description

/** \brief Calculate buffer size from dlen header member */
#define BUFFER_SIZE_FROM_DLEN(dlen)          ( (dlen) * 2 + sizeof (s_bufhe_host))
#define BUFFER_SIZE_FROM_DLEN_BROKEN(dlen)   ( (dlen) * 2 )
/** \brief Calculate number of bytes used of data block in buffer from iused */
#define BUFFER_USED_FROM_IUSED(iused)        ( (iused) * 2)
/** \brief Calculate data length of event from dlen header member */
#define EVENT_DATA_LENGTH_FROM_DLEN(dlen)    ( (dlen) * 2)
/** \brief Calculate data length of event from dlen header member */
#define SUBEVENT_DATA_LENGTH_FROM_DLEN(dlen) ( ((dlen) - 2) * 2)

/** \brief Calculate dlen header member from buffer size */
#define DLEN_FROM_BUFFER_SIZE(size)          ( (size -  sizeof (s_bufhe_host)) / 2)
/** \brief Calculate iused from number of bytes used of data block in buffer */
#define IUSED_FROM_BUFFER_USED(used)         ( (used) / 2)
/** \brief Calculate dlen header member from data length of event */
#define DLEN_FROM_EVENT_DATA_LENGTH(length)  ( (length) / 2)
/** \brief Calculate data length of event from dlen header member */
#define SUBEVENT_DLEN_FROM_DATA_LENGTH(length) ( ((length) / 2) + 2)

/** \brief Calculate data length of event from dlen header member */
#define SUBEVENT_DATA_LENGTH_FROM_DLEN_X(dlen) ( (dlen) * 2)

/** \brief Calculate data length of event from dlen header member */
#define SUBEVENT_DLEN_FROM_DATA_LENGTH_X(length) ( (length) / 2)

///////////////////////////////////////////////////////
// swap between little and big endian formats

// Swap data structures (in place)

void byteswap_32(void* ptr,size_t len);

void byteswap_32(s_bufhe_host& buf_header);
void byteswap_32(s_filhe_extra_host &file_extra);

void byteswap_32(lmd_event_header_host& header);

//void unaligned_ntoh(lmd_event_header_host &header,const uchar* src);
//void aligned_dlen_hton(uchar *dest,const lmd_event_header_host& header,INTS4 l_dlen);

///////////////////////////////////////////////////////

void zero_padding(s_filhe_extra_host &file_extra);

///////////////////////////////////////////////////////

// For internal use in ucesb only!

#define LMD_DUMMY_BUFFER_MARK_TYPE     0x2c7a // randomly chosen
#define LMD_DUMMY_BUFFER_MARK_SUBTYPE  0x28ae
#define LMD_DUMMY_BUFFER_MARK_FREE_3   0x2faea5a9

///////////////////////////////////////////////////////

#endif// __LMD_EVENT_H__
