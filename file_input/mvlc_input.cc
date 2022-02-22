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

#include "mvlc_input.hh"

#include "error.hh"

#include "config.hh"
#include "hex_dump.hh"
#include "util.hh"

#include <time.h>

#include "thread_buffer.hh"
#include "worker_thread.hh"

#include <algorithm>

mvlc_source::mvlc_source() {}

void mvlc_source::new_file() {
  _last_seq_no = 0;

  // _chunk_cur = _chunk_end = NULL;
}

#if !USE_THREADING && !USE_MERGING
FILE_INPUT_EVENT _file_event;
#endif

#define mvlc_swap(data)                                                        \
  if (_swapping) {                                                             \
    byteswap((uint32 *)&(data), sizeof(data));                                 \
  }

bool mvlc_source::read_preamble_endianness(mvlc_event *dest) {
  if (!_input.read_range(&_preamble, sizeof(_preamble)))
    return false;

  for (size_t i = 0; i <= 7; i++) {
    if (_preamble.magic[i] != mvlc_magic[i]) {
      ERROR("Invalid file header: '%.8s'", _preamble.magic);
      return false;
    }
  }

  mvlc_frame<1> endian_frame;
  if (!_input.read_range(&endian_frame, sizeof(endian_frame)))
    return false;

  _swapping = (endian_frame.data[0] != MVLC_ENDIAN_MARKER);
  mvlc_swap(endian_frame);

  if ((endian_frame.system_event_header.subtype() !=
       mvlc_system_event_subtype_endian_marker) ||
      (endian_frame.data[0] != MVLC_ENDIAN_MARKER)) {
    ERROR("Invalid endian marker frame: %08x %08x, subtype = 0x%02x",
          endian_frame.header, endian_frame.data[0],
          endian_frame.system_event_header.subtype());
    return false;
  }
  return true;
}

// TODO: Don’t read_range, map_range
bool mvlc_source::next_eth_frame(mvlc_event *dest) {
  for (;;) {
    if (!_input.read_range(&eth_header.header0, sizeof(eth_header.header0)))
      return false;

    mvlc_swap(eth_header.header0);

    if (LIKELY(eth_header.header0.channel() == mvlc_eth_channel_data)) {
      if (!_input.read_range(&eth_header.header1, sizeof(eth_header.header1)))
        return false;
      mvlc_swap(eth_header.header1);

      // assert(eth_header.header1.cont() == 0);
      eth_header.unused =
          static_cast<sint32>(eth_header.header0.length() * sizeof(uint32_t));

      return true;
    } else if (UNLIKELY(eth_header.header0.event_type() ==
                        mvlc_system_event_subtype_end_of_file)) {
      return false;
    }

    // Map the data to skip it
    _input.map_range(eth_header.header0.length() * sizeof(uint32_t),
                     dest->_chunks);
  }
}

// TODO: Don’t read_range, map_range
bool mvlc_source::next_stack_frame(mvlc_event *dest) {
  while (eth_header.unused > 0 || next_eth_frame(dest)) {
    if (!_input.read_range(&dest->_header, sizeof(dest->_header)))
      return false;
    mvlc_swap(dest->_header);

    size_t data_size = dest->_header.frame_header.length() * sizeof(uint32_t);
    eth_header.unused -= static_cast<sint32>(data_size + sizeof(dest->_header));

    dest->_frame_end = data_size + eth_header.unused;

    if (dest->_header.frame_header.type() == mvlc_frame_stack_frame) {
      return true;
    }

    // Map the data to skip it
    _input.map_range(data_size + (eth_header.unused < 0) ? eth_header.unused
                                                         : 0,
                     dest->_chunks);
  }
  return false;
}

mvlc_event *mvlc_source::get_event() {
  mvlc_event *dest;

#ifdef USE_THREADING
  // Even if we blow up with an error, the reclaim item will be in the
  // reclaim list, so the memory wont leak
  dest = (mvlc_event *)_wt._defrag_buffer->allocate_reclaim(sizeof(mvlc_event));
#else
  _file_event.release();
  dest = &_file_event;
#endif

  if (UNLIKELY(_preamble.magic[0] != 'M' && !read_preamble_endianness(dest)))
    return NULL;

  dest->_status = 0;
  dest->_subevents = NULL;
  dest->_swapping = _swapping;

  _input.release_to(_input._cur);

  // Go to next stack frame
  if (!next_stack_frame(dest))
    return NULL;

  // So header is safely retrieved.

  // Now check that it makes sense
  size_t data_size = dest->_header.frame_header.length() * sizeof(uint32_t);
  size_t data_size_full = data_size;
  if (UNLIKELY(data_size > MVLC_SECTION_MAX_LENGTH))
    ERROR("Data length (0x%08lx) very large, refusing.", data_size);

  // And then retrieve the data itself.
  dest->_chunk_cur = dest->_chunk_end = dest->_chunks;
  dest->_offset_cur = 0;

  if (UNLIKELY(0 > eth_header.unused)) {
    // TODO: instead of _input.map_range, copy/move data in current ethernet
    // frame to new buffer, go to to next ethernet frame, release previous
    // frame, copy appropriate amount of data, and never worry about old frame
    // again.
    // printf("data_size = %lu, unused = %d, cur = %lu\n", data_size,
    //        eth_header.unused, _input._cur);
    // printf("Segfault imminent, goodbye, cruel unpacker!\n");
    // fflush(stdout);

    // Map the range
    uint32 first = static_cast<uint32>(data_size + eth_header.unused);
    uint32 next = static_cast<uint32>(-eth_header.unused);
#ifdef USE_THREADING
    char *defrag = (char *)_wt._defrag_buffer->allocate_reclaim(data_size);
#else
    char *defrag = (char *)dest->_defrag_event_many.allocate(data_size);
#endif
    dest->_data = defrag;
    char *defrag_cur = defrag;

    if (UNLIKELY(!_input.read_range(defrag_cur, first))) {
      return NULL;
    }

    data_size -= first;
    defrag_cur += first;

    while (data_size > 0) {
      if (!next_eth_frame(dest)) {
        return NULL;
      }
      eth_header.unused -= next;
      if (eth_header.unused < 0) {
        first =
            static_cast<uint32>(eth_header.header0.length() * sizeof(uint32_t));
      } else {
        first = next;
      }
      if (UNLIKELY(!_input.read_range(defrag_cur, first))) {
        return NULL;
      }
      defrag_cur += first;
      data_size -= first;
    }
    dest->_chunks[0]._ptr = dest->_data;
    dest->_chunks[0]._length = data_size_full;
    dest->_chunks[1]._ptr = NULL;
    dest->_chunks[1]._length = 0;
    dest->_chunk_cur = &(dest->_chunks[0]);
    dest->_chunk_end = &(dest->_chunks[1]);
    // dest->_chunk_end->_ptr = NULL;
    // dest->_chunk_end->_length = 0;
  } else if (LIKELY(data_size)) {
    // Do not retrieve any data if the data size is 0 (or locate_subevents
    // will try to find a first header...
    int chunks = 0;

    chunks = _input.map_range(data_size, dest->_chunks);

    if (UNLIKELY(!chunks)) {
      ERROR("Error while reading subevent data.");
      return NULL;
    }

    dest->_chunk_end += chunks;
  }

  // ok, so data for event is in the chunks

  return dest;
}

// Event Section:----------- Crate:---------- Event Type:----- Size:--------
//   SubEv                                   Module Type:----- Size:--------

void mvlc_event::print_event(int data, hex_dump_mark_buf *unpack_fail) const {
  printf(
      "Event Type:   %s0x%02x%s    Flags: %s%d%d%d%d%s Ctrl ID: %s%2u%s    "
      "Stack "
      "Num: %s%2u%s    "
      "Size:%s%8lu%s\n",
      CT_OUT(BOLD_BLUE), _header.frame_header.type(), CT_OUT(NORM_DEF_COL),
      CT_OUT(BOLD_BLUE), _header.frame_header.cont(),
      _header.frame_header.syntax_error(), _header.frame_header.bus_error(),
      _header.frame_header.time_out(), CT_OUT(NORM_DEF_COL), CT_OUT(BOLD_BLUE),
      _header.frame_header.ctrl_id(), CT_OUT(NORM_DEF_COL), CT_OUT(BOLD_BLUE),
      _header.frame_header.stack_num(), CT_OUT(NORM_DEF_COL), CT_OUT(BOLD_BLUE),
      _header.system_event_header.length() * sizeof(uint32_t),
      CT_OUT(NORM_DEF_COL));

  // Subevents
  if (_header.frame_header.type() == mvlc_frame_stack_frame) {
    for (int subevent = 0; subevent < _nsubevents; subevent++) {
      mvlc_subevent *subevent_info = &_subevents[subevent];

      bool subevent_error =
          (unpack_fail && unpack_fail->_next == &subevent_info->_header);

      printf("  %sSubEv%s Type: %s0x%02x%s                Ctrl ID: %s%2u%s    "
             "Stack Num: %s%2u%s    "
             "Size:%s%8lu%s\n",
             subevent_error ? CT_OUT(BOLD_RED) : "",
             subevent_error ? CT_OUT(NORM_DEF_COL) : "", CT_OUT(BOLD_MAGENTA),
             subevent_info->_header.frame_header.type(), CT_OUT(NORM_DEF_COL),
             CT_OUT(BOLD_MAGENTA),
             subevent_info->_header.frame_header.ctrl_id(),
             CT_OUT(NORM_DEF_COL), CT_OUT(BOLD_MAGENTA),
             subevent_info->_header.frame_header.stack_num(),
             CT_OUT(NORM_DEF_COL), CT_OUT(BOLD_MAGENTA),
             subevent_info->_header.frame_header.length() * sizeof(uint32_t),
             CT_OUT(NORM_DEF_COL));

      if (data) {
        hex_dump_buf buf;

        if (subevent_info->_data) {
          uint32 data_length = static_cast<uint32>(
              subevent_info->_header.frame_header.length() * sizeof(uint32));

          hex_dump(stdout, (uint32 *)subevent_info->_data,
                   subevent_info->_data + data_length, subevent_info->_swapping,
                   "%c%08x", 9, buf, unpack_fail);
        }
      }
    }
  }
  // TODO: Print data of non-event sections

  // Is there any remaining data, that could not be assigned as a subevent?

  print_remaining_event(_chunk_cur, _chunk_end, _offset_cur,
                        0 /*subevent_info->_swapping*/);
}

void mvlc_event::locate_subevents(mvlc_event_hint * /*hints*/) {
  if (_status & MVLC_EVENT_LOCATE_SUBEVENTS_ATTEMPT)
    return;

  _status |= MVLC_EVENT_LOCATE_SUBEVENTS_ATTEMPT;

  if (_header.frame_header.type() != mvlc_frame_stack_frame) {
    ERROR("Unrecognized frame type %02x.", _header.frame_header.type());
    return;
  }

  assert(!_subevents); // we have already been called!

  // get some space, for at least so many subevents as we have seen at
  // most before...

#define MAX_SUBEVENTS 64 // TODO: fix this!!!

#ifdef USE_THREADING
  _subevents = (mvlc_subevent *)_wt._defrag_buffer->allocate_reclaim(
      sizeof(mvlc_subevent) * MAX_SUBEVENTS);
#else
  _subevents = (mvlc_subevent *)_defrag_event.allocate(sizeof(mvlc_subevent) *
                                                       MAX_SUBEVENTS);
#endif

  _nsubevents = 0;

  // Then, let's chew over the event...

  buf_chunk *chunk_cur = _chunk_cur;
  size_t offset_cur = _offset_cur;

  // void *start = fragment->_ptr;
  // size_t left;

  while (chunk_cur < _chunk_end) {
    // First of all, do we have space to dump this subevent?
    /*
    printf ("%x / %x (%d)\n",
            offset_cur,chunk_cur->_length,
            _chunk_end - chunk_cur);
    fflush(stdout);
    */

    // We have more data to read.
    // Does this fragment have enough data for the subevent header?

    mvlc_subevent *subevent_info = &_subevents[_nsubevents];
    mvlc_frame<0> *subevent_header = &subevent_info->_header;

    if (!get_range_many((char *)subevent_header, chunk_cur, offset_cur,
                        _chunk_end, sizeof(mvlc_frame<0>))) {
      // We're simply out of data.
      ERROR("Subevent header not complete before end of event.");
    }

    subevent_info->_swapping = _swapping;
    mvlc_swap(subevent_info->_header);

    // So, subevent header is there, now remember where we got the
    // data

    size_t data_length =
        subevent_header->frame_header.length() * sizeof(uint32_t);

    // Did the data come exclusively from this buffer?

    size_t chunk_left =
        chunk_cur < _chunk_end ? chunk_cur->_length - offset_cur : 0;

    // printf("header: 0x%08x data_length: %lu, chunk_left: %lu\n",
    //        subevent_header->frame_header.data, data_length, chunk_left);
    // fflush(stdout);

    // Check if there is data beyond end of buffers
    if (LIKELY(data_length <= chunk_left)) {
      // likely, since most subevents smaller than buffer size

      if (UNLIKELY(!data_length)) {
        // if the data size is zero, and we're the last
        // subevent, then chunk_cur will not be valid, and we
        // should not give such a pointer away (i.e. we may not
        // dereference chunk_cur, so we cannot give it away)

        // But we want to give an invalid (but non-null) pointer
        // (since the length is zero, noone should _ever_ look
        // at it...

        subevent_info->_data = (char *)16;
      } else {
        subevent_info->_data = chunk_cur->_ptr + offset_cur;
      }

      offset_cur += data_length;
    } else {
      // We have some fun, first of all, the data is not stored
      // continous yet

      subevent_info->_data = NULL;

      // This is where to get to the data

      subevent_info->_frag = chunk_cur;
      subevent_info->_offset = offset_cur;

      // Second of all, we must make sure that one cat get to
      // the data at all!  (we must anyhow read past it to get to
      // the next subevent in the buffer)

      // Each time we come into the loop, the current fragment
      // has some data, but not enough

      data_length -= chunk_left;

      // So, go get next fragment

      chunk_cur++;

      // if (chunk_cur >= _chunk_end || data_length > chunk_cur->_length) {
      //   ERROR("Subevent (module type 0x%08x) length %lu bytes, "
      //         "not complete before end of event, %d bytes missing",
      //         subevent_info->_header._info._module_type,
      //         subevent_info->_header._info._size * sizeof(uint32_t),
      //         (int)data_length);
      // }

      chunk_left = chunk_cur->_length;

      offset_cur = data_length;

      // Account for how much data was used in ending fragment
    }

    // One more subevent fitted in the event...

    _nsubevents++;

    // Now, we might exactly have reached a fragment boundary
    // This we need not consider, since it would be handled at
    // the beginning of the loop as an fragmented subevent header!
    // But then, we'll at some times activate the branch for
    // fragmented headers.  Better let it be handled here

    if (chunk_left == data_length) {
      chunk_cur++;
      offset_cur = 0;
    }

    // Subevent was successfully parsed, note that...

    _chunk_cur = chunk_cur;
    _offset_cur = offset_cur;
  }

  // printf ("===> %d\n",_nsubevents);
  //   fflush(stdout);

  // Aha, so we have successfully gotten all subevent data
}

void mvlc_event::get_subevent_data_src(mvlc_subevent *subevent_info,
                                       char *&start, char *&end) {
  // Now, if we would be running on a Cell SPU, we would be
  // wanting to get the data from the main memory in any case,
  // with DMA, so we can handle the fragmented cases just as well
  // as the non-fragmented.  It is just a few more DMA operations.
  // So that one would use a special function, and not this one.
  // This one is for use by normal processors.

  // We have a few cases.  Either the data was not fragmented:

  size_t data_length =
      subevent_info->_header.frame_header.length() * sizeof(uint32_t);

  if (UNLIKELY(subevent_info->_data == NULL)) {
    // We need to copy the subevent to an defragment buffer.
    // The trouble with this buffer is that it needs to be
    // deallocated or marked free somehow also...

    // Defragmentation buffers.  Need to survive until we have
    // retired the event (retire = ALL operations done).  This
    // is since we may be requested to do a data dump of an
    // (broken) event (or a normal event dump).  However, since
    // different processes might be responsible for allocating
    // the defrag buffer (either the main file reading one (due
    // to event printout), or the unpacking one (due to unpack)
    // or the retirement one (due to debug printout), we need to
    // have one defrag buffer pool per such process, since
    // allocating should not need to wait for anyone else.  The
    // pointer supplied will be stable for the rest of the event
    // life-time, i.e. can be used by other threads on the same
    // machine.

#ifdef USE_THREADING
    char *defrag = (char *)_wt._defrag_buffer->allocate_reclaim(data_length);
#else
    char *defrag = (char *)_defrag_event_many.allocate(data_length);
#endif

    subevent_info->_data = defrag;

    buf_chunk *frag = subevent_info->_frag;

    size_t size0 = frag->_length - subevent_info->_offset;

    // First copy data from first fragmented buffer

    memcpy(defrag, frag->_ptr + subevent_info->_offset, size0);

    defrag += size0;
    size_t length = data_length - size0;

    frag++;

    // And copy last one

    assert(length <= frag->_length);

    memcpy(defrag, frag->_ptr, length);
    /*
      for (uint i = 0; i < subevent_info->_length; i++)
      printf ("%02x ",(uint8) subevent_info->_data[i]);
      printf ("\n");
      fflush(stdout);
    */
  }

  start = subevent_info->_data;
  end = subevent_info->_data + data_length;

  return;
}
