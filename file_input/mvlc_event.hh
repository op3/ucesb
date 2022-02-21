/* This file is part of UCESB - a tool for data unpacking and processing.
 *
 * Copyright (C) 2016  GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
 * Copyright (C) 2022  Oliver Papst
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

#ifndef __MVLC_EVENT_HH__
#define __MVLC_EVENT_HH__

//#define MVLC_RECORD_ID "MVLCDATA"

// 20 bit of uint32 (without header)
#define MVLC_SECTION_MAX_LENGTH 0x400000
#define MVLC_ENDIAN_MARKER 0x12345678

struct mvlc_preamble {
  char magic[8];
};

static const char mvlc_magic[8] = {'M', 'V', 'L', 'C', '_', 'E', 'T', 'H'};

#define mvlc_attr(name, pos, size)                                             \
  inline uint32 name() const { return (data >> pos) & ((1 << size) - 1); }

struct mvlc_system_event_header {
  uint32 data;
  mvlc_attr(event_type, 24, 8);
  mvlc_attr(cont, 23, 1);
  mvlc_attr(ctrl_id, 20, 3);
  mvlc_attr(subtype, 13, 7);
  mvlc_attr(length, 0, 13);
};

struct mvlc_ethernet_frame_header0 {
  uint32 data;
  mvlc_attr(channel, 28, 4);
  mvlc_attr(packet_number, 16, 12);
  mvlc_attr(ctrl_id, 13, 3);
  mvlc_attr(length, 0, 13);
};

struct mvlc_ethernet_frame_header1 {
  uint32 data;
  mvlc_attr(timestamp, 12, 20);
  mvlc_attr(cont, 0, 12);
};

struct mvlc_frame_header {
  uint32 data;
  mvlc_attr(type, 24, 8);
  mvlc_attr(cont, 23, 1);
  mvlc_attr(syntax_error, 22, 1);
  mvlc_attr(bus_error, 21, 1);
  mvlc_attr(time_out, 20, 1);
  mvlc_attr(flags, 20, 4);
  mvlc_attr(stack_num, 16, 4);
  mvlc_attr(ctrl_id, 13, 3);
  mvlc_attr(length, 0, 13);
};

enum mvlc_frame_types {
  mvlc_frame_super_frame = 0xf1,
  mvlc_frame_stack_frame = 0xf3,
  mvlc_frame_block_read = 0xf5,
  mvlc_frame_stack_error = 0xf7,
  mvlc_frame_stack_continuation = 0xf9,
  mvlc_frame_system_event = 0xfa,
};

enum mvlc_system_event_subtype {
  mvlc_system_event_subtype_endian_marker = 0x01,
  mvlc_system_event_subtype_begin_run = 0x02,
  mvlc_system_event_subtype_end_run = 0x03,
  mvlc_system_event_subtype_mvme_config = 0x10,
  mvlc_system_event_subtype_unix_timetick = 0x11,
  mvlc_system_event_subtype_pause = 0x12,
  mvlc_system_event_subtype_resume = 0x13,
  mvlc_system_event_subtype_mvlc_crate_config = 0x14,
  mvlc_system_event_subtype_end_of_file = 0x77,
  mvlc_system_event_subtype_max = 0x7f,
};

enum mvlc_eth_channel {
  mvlc_eth_channel_command = 0x0,
  mvlc_eth_channel_stack = 0x1,
  mvlc_eth_channel_data = 0x2,
};

struct mvlc_ethernet_frame_header {
  mvlc_ethernet_frame_header0 header0;
  mvlc_ethernet_frame_header1 header1;
  sint32 unused;
};

template <size_t N> struct mvlc_frame {
  union {
    mvlc_system_event_header system_event_header;
    mvlc_frame_header frame_header;
    struct {
      uint16 type : 8;
      uint32 : 24;
    };
    uint32 header;
  };
  uint32 data[N];
};

typedef uint32 mvlc_data_t;

#endif //__MVLC_EVENT_HH__
