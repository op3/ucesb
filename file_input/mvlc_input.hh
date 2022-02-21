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

#ifndef __MVLC_INPUT_HH__
#define __MVLC_INPUT_HH__

#include "decompress.hh"
#include "hex_dump_mark.hh"
#include "input_buffer.hh"
#include "input_event.hh"

#include "typedef.hh"
#include <stdlib.h>

#ifdef USE_MVLC_INPUT

#include "swapping.hh"

#include "mvlc_event.hh"

// The mvlc event data  does
// in contrast to other file formats not contain any buffers or
// records.  The events are located directly in the files.  Some
// carefulness is thus required as the events (and headers) may span
// different input chunks.

#define MVLC_EVENT_DECODING_SWAP_A 0xff000000
#define MVLC_EVENT_DECODING_SWAP_B 0x000000ff

class mvlc_source;

struct mvlc_subevent {
  mvlc_frame<0> _header;

  char *_data;      // ptr to data in contigous memory
  buf_chunk *_frag; // where data is (fragmented)
  size_t _offset;   // offset into fragment

  bool _swapping;
};

#define MVLC_EVENT_LOCATE_SUBEVENTS_ATTEMPT 0x0004
#define MVLC_EVENT_SUBEVENTS_LEFTOVERS                                         \
  0x0008 // could not read subevents to end...

struct mvlc_event_hint // see lmd_event_hint (do the copy-paste...)
{};

struct mvlc_event : public input_event {
  mvlc_frame<0> _header;

  int _status;
  bool _swapping;

  /* Whenever someone has gone through the trouble to locate the
   * subevents (all _will_ be done at once, we get a pointer to that
   * array of information.
   */

  mvlc_subevent *_subevents;
  int _nsubevents; // not valid until _subevents has been set

  /* MVLC events span multiple buffers.
   */

  buf_chunk *_chunk_end; // the current end of chunks
  buf_chunk *_chunk_cur; // the current end of chunks
  size_t _frame_end;
  size_t _offset_cur;

  mvlc_source *src;

#ifndef USE_THREADING
public:
  keep_buffer_many _defrag_event_many;

public:
  void release() {
    input_event::release();
    _defrag_event_many.release();
  }
#endif

  // As opposed to LMD events, this must not necessarily be the last
  // members, as we at most have two chunks

  buf_chunk _chunks[2];

public:
  void locate_subevents(mvlc_event_hint *hints);
  void get_subevent_data_src(mvlc_subevent *subevent_info, char *&start,
                             char *&end);
  void print_event(int data, hex_dump_mark_buf *unpack_fail) const;
};

#define FILE_INPUT_EVENT mvlc_event

#ifndef USE_THREADING
extern FILE_INPUT_EVENT _file_event;
#endif

class mvlc_source : public data_input_source {
public:
  mvlc_source();
  ~mvlc_source() {}

public:
  void new_file();

public:
  int _last_seq_no;
  mvlc_preamble _preamble;
  mvlc_ethernet_frame_header eth_header;
  bool _swapping;

  /*
public:
  buf_chunk  _chunks[2];
  buf_chunk *_chunk_cur;
  buf_chunk *_chunk_end;
  */

public:
  mvlc_event *get_event(/*mvlc_event *dest*/);

public:
  void print_file_header();
  // void print_buffer_header();

public:
  bool read_record();
  bool skip_record();
  bool next_eth_frame(mvlc_event *);

private:
  bool read_preamble_endianness(mvlc_event *);
  bool next_stack_frame(mvlc_event *);
};

#endif // USE_MVLC_INPUT

#endif //__MVLC_INPUT_HH__
