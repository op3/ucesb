/* This file is part of UCESB - a tool for data unpacking and processing.
 *
 * Copyright (C) 2020  Haakan T. Johansson  <f96hajo@chalmers.se>
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

#define DO_EXT_NET_DECL
#include "ext_file_writer.hh"

#include "ext_file_error.hh"

#include <string.h>
#include <assert.h>
#include <arpa/inet.h>

extern const char *_argv0;
extern int _got_sigio;

#define DO_MRG_DBG 0

#if DO_MRG_DBG
#define MRG_DBG(...) {				\
    fprintf(stdout,__VA_ARGS__);		\
  }
#else
#define MRG_DBG(...) ((void) 0)
#endif


/* Handling of merging (time stitching) of events.
 *
 * All events are handed over to us, and not processed further.
 * We may however as the result of such a hand-over event emit one
 * or more events, for further processing.
 *
 * The assumption is that events are delivered with increasing
 * timestamps for each source identifier.
 *
 * And that events which have multi-event marker 0 (i.e. are the first
 * that comes from an original collection of events from the same
 * source) are in order relative to all other source IDs with
 * multi-event marker 0.
 *
 * Therefore, whenever a event with multi-event mark 0 arrives, all
 * earlier events are merged, since it the ordering means that no
 * other source can contribute earlier events.
 *
 * In case a source provides events with broken timestamps, those
 * events are dumped immediately.  Note! they will even be dumped
 * before events from the same source id with earlier timestamps!
 * This to allow such earlier events to still merge with events from
 * other sources that may not have arrived yet.  Since we do not know
 * with the global (i.e. multi-event marker 0) timestamp would have
 * been, we do not know how far in time the original time-sorter has
 * reached.  This reordering allows to most correct events to be
 * merged.  Events with broken timestamps can anyhow not be merged
 * with anything.

 */

struct merge_item_incl
{
  uint32_t *_p;
  uint32_t *_pend;
};

struct merge_result
{
  uint32_t  _flags;
};

/* One event, pre-merge. */
struct merge_item_chunk
{
  //uint32_t  _offset_next;
  uint32_t  _tstamp_lo;
  uint32_t  _tstamp_hi;
  uint32_t  _prelen;
  uint32_t  _plen;

  uint32_t  _flags;
};

/* Store for data from one srcid. */
struct merge_item_store
{
  //uint32_t  _events;
  uint32_t  _offset_first;
  //uint32_t  _offset_last;
  uint32_t  _alloc;
  uint32_t  _used;
  void     *_buf;

};


merge_item_store **_items_store = NULL;
size_t             _alloc_items_store = 0;

merge_item_incl   *_merge_incl = NULL;
size_t             _num_merge_incl;

uint64_t toldest_cur = (uint64_t) -1;

uint32_t *_merge_dest = NULL;
size_t    _merge_dest_alloc = 0;

void ext_merge_item(uint32_t mark, uint32_t *pp,
		    merge_result *result)
{
  uint32_t clear_zero =
    mark & EXTERNAL_WRITER_MARK_CLEAR_ZERO;

  /* For a normal item, we only want to find (at most) one source
   * having a non-zero item.
   */

  if (clear_zero)
    {
      size_t s;

      for (s = 0; s < _num_merge_incl; s++)
	{
	  merge_item_incl *incl = &_merge_incl[s];
	  MRG_DBG("i0 s: %zd p: %p *p: %08x\n", s, incl->_p, *incl->_p);
	  uint32_t val = *(incl->_p++);

	  /* We always set the value.  Cheaper than a branch,
	   * and also ensures that we set also a zero value.
	   */
	  *pp = val;

	  if (val)
	    break;
	}
      /* We have either reached the end of the list, or set
       * the value at least once.  If we find another non-zero
       * value, we have a problem.
       */
      for (s++ ; s < _num_merge_incl; s++)
	{
	  merge_item_incl *incl = &_merge_incl[s];
	  MRG_DBG("i1 s: %zd p: %p *p: %08x\n", s, incl->_p, *incl->_p);
	  uint32_t val = *(incl->_p++);

	  if (val &&
	      !(mark & EXTERNAL_WRITER_MARK_MULT_NON0))
	    result->_flags |= EXT_FILE_MERGE_MULTIPLE_IVALUE;
	  /* We must anyhow go through all sources, to increment
	   * their pointers.
	   */
	}
    }
  else
    {
      /* Floating-point.  All except one should be NaN. */
      /* Same logic as above. */

      size_t s;

      for (s = 0; s < _num_merge_incl; s++)
	{
	  merge_item_incl *incl = &_merge_incl[s];
	  MRG_DBG("f0 s: %zd p: %p *p: %08x\n", s, incl->_p, *incl->_p);
	  uint32_t val = *(incl->_p++);

	  *pp = val;

	  val = ntohl(val);

	  if ((val & 0x7f800000) == 0x7f800000)
	    break;
	}
      for (s++ ; s < _num_merge_incl; s++)
	{
	  merge_item_incl *incl = &_merge_incl[s];
	  MRG_DBG("f1 s: %zd p: %p *p: %08x\n", s, incl->_p, *incl->_p);
	  uint32_t val = *(incl->_p++);

	  val = ntohl(val);

	  if ((val & 0x7f800000) == 0x7f800000 &&
	      !(mark & EXTERNAL_WRITER_MARK_MULT_NON0))
	    result->_flags |= EXT_FILE_MERGE_MULTIPLE_FVALUE;
	}
    }

}

void ext_merge_array(uint32_t **ptr_o,
		     uint32_t **ptr_pp,
		     merge_result *result)
{
  uint32_t *o  = *ptr_o;
  uint32_t *pp = *ptr_pp;

  uint32_t max_loops = *(o++);
  uint32_t loop_size = *(o++);

  uint32_t *onext = o + 2 * max_loops * loop_size;

  MRG_DBG("loop: %d * %d = %d\n",
	  max_loops, loop_size, max_loops * loop_size);

  /* For lists (loops) we add data from all sources,
   * individually.  Thus the peril is if there are too many
   * items, which cannot be accomodated.
   *
   * We also want keep the sorting of items, in case they were
   * delivered in index order.
   */

  /* Remember the address of the destination controlling item. */
  uint32_t *pploop = pp++;

  uint32_t loops = 0;
  size_t s;

  for (s = 0; s < _num_merge_incl; s++)
    {
      merge_item_incl *incl = &_merge_incl[s];

      MRG_DBG("l0 s: %zd p: %p *p: %08x\n", s, incl->_p, *incl->_p);

      uint32_t val = htonl(*(incl->_p++));

      uint32_t use_loops;

      if (loops + val > max_loops)
	{
	  use_loops = max_loops - loops;
	  result->_flags |= EXT_FILE_MERGE_ARRAY_OVERFLOW;
	}
      else
	use_loops = val;

      loops += use_loops;

      uint32_t skip_loops = val - use_loops;

      uint32_t items = use_loops * loop_size;
      uint32_t skip_items = skip_loops * loop_size;

      MRG_DBG("loop: v:%d use: %d items: %d skip_items: %d\n",
	      val, use_loops, items, skip_items);

      uint32_t i;

      for (i = items; i; i--)
	{
	  uint32_t item_mark = *(o++);
	  uint32_t item_offset = *(o++);

	  (void) item_mark;
	  (void) item_offset;

	  MRG_DBG("l1 s: %zd p: %p *p: %08x\n", s, incl->_p, *incl->_p);

	  uint32_t val = *(incl->_p++);

	  /* Simply copy the item. */
	  *(pp++) = val;
	}

      /* Jump past the items that would not fit. */
      incl->_p += skip_items;
    }

  *pploop = ntohl(loops);

  o = onext;

  *ptr_o  = o;
  *ptr_pp = pp;
}

uint32_t *ext_merge_do_merge(offset_array *oa,
			     uint32_t *pdest,
			     merge_result *result)
{
  /* We go through the items according to the pack list.
   *
   * The items are then combined.
   */

  uint32_t *o    = oa->_ptr;
  uint32_t *oend = oa->_ptr + oa->_length;

  uint32_t *pp = pdest;

  size_t s;

#if DO_MRG_DBG
  for (s = 0; s < _num_merge_incl; s++)
    {
      merge_item_incl *incl = &_merge_incl[s];

      MRG_DBG ("Source %zd: %p - %p\n",
	       s, incl->_p, incl->_pend);
    }
#endif

  while (o < oend)
    {
      uint32_t mark   = *(o++);
      uint32_t offset = *(o++);
      uint32_t loop = mark & EXTERNAL_WRITER_MARK_LOOP;

      MRG_DBG("[%4zd]: %08x %08x\n",
	      o - 2 - oa->_ptr, mark, offset);

      (void) offset;

      if (!loop)
	ext_merge_item(mark, pp++, result);
      else
	ext_merge_array(&o, &pp, result);
    }

  /* Check that all sources reached their end exactly. */

  for (s = 0; s < _num_merge_incl; s++)
    {
      merge_item_incl *incl = &_merge_incl[s];

      if (incl->_p != incl->_pend)
	ERR_MSG("Source (%zd) not used fully for merging, %zd items left.",
		s, incl->_pend - incl->_p);
    }

  MRG_DBG ("merged: %zd\n", pp - pdest);

  /* result->_flags = 7; */

  if (oa->_poffset_mrg_stat != (uint32_t) -1)
    pdest[oa->_poffset_mrg_stat] = htonl(result->_flags);

  return pp;
}


bool ext_merge_sort_until(ext_write_config_comm *comm,
			  offset_array *oa,
			  uint64_t t_until,
			  uint32_t maxdestplen)
{
  size_t i;

  merge_result result;

  /* Look through the chunks we have to find the oldest item. */
  uint64_t toldest = (uint64_t) -1;

  for (i = 0; i < _alloc_items_store; i++)
    {
      merge_item_store *store = _items_store[i];

      if (store &&
	  store->_offset_first < store->_used)
	{
	  merge_item_chunk *item =
	    (merge_item_chunk*) (((char *) store->_buf) +
				 store->_offset_first);
	  uint64_t tstamp =
	    (((uint64_t) item->_tstamp_hi) << 32) + item->_tstamp_lo;

	  if (tstamp < toldest)
	    toldest = tstamp;
	}
    }

  MRG_DBG ("merge_until %016llx , oldest: %016llx\n",
	   t_until, toldest);

  if (toldest >= t_until)
    {
      /* We cannot sort this yet. */
      return false;
    }

  MRG_DBG ("-------------------------------------------------------\n");
  MRG_DBG ("merge %016llx...\n",
	   toldest);

  /* Look through the chunks to find which can contribute an event
   * (i.e. are within window).
   */

  uint64_t twindow_end = toldest + _config._ts_merge_window;

  _num_merge_incl = 0;

  char    *incl0_msg = NULL;
  uint32_t incl0_prelen = 0;
  uint64_t incl0_tstamp = 0;

  result._flags = 0;

  for (i = 0; i < _alloc_items_store; i++)
    {
      merge_item_store *store = _items_store[i];

      if (store &&
	  store->_offset_first < store->_used)
	{
	  merge_item_chunk *item =
	    (merge_item_chunk*) (((char *) store->_buf) +
				 store->_offset_first);
	  uint64_t tstamp =
	    (((uint64_t) item->_tstamp_hi) << 32) + item->_tstamp_lo;

	  if (tstamp > twindow_end)
	    {
	      /* The event is too new, not included in merge. */
	      /* But perhaps it is the oldest of the next one? */
	      if (tstamp < toldest_cur)
		toldest_cur = tstamp;
	      continue;
	    }

	  merge_item_incl *incl = &_merge_incl[_num_merge_incl++];
	  char *msg = (char *) (item + 1);

	  if (!incl0_msg ||
	      tstamp < incl0_tstamp)
	    {
	      incl0_msg    = msg;
	      incl0_prelen = item->_prelen;
	      incl0_tstamp = tstamp;
	    }

	  /* The item data begins after the header. */
	  incl->_p = (uint32_t *) (msg + item->_prelen);
	  incl->_pend = (uint32_t *) (((char *) incl->_p) + item->_plen);

	  /* Move the store forward. */
	  store->_offset_first +=
	    sizeof (merge_item_chunk) + item->_prelen + item->_plen;
	  // store->_events--;

	  /* If there is a next item in the store, we check that its
	   * time is not also within the window, which we will
	   * dislike (flag).
	   */
	  if (store->_offset_first >= store->_used)
	    continue;

	  merge_item_chunk *item2 =
	    (merge_item_chunk*) (((char *) store->_buf) +
				     store->_offset_first);
	  uint64_t tstamp2 =
	    (((uint64_t) item2->_tstamp_hi) << 32) + item2->_tstamp_lo;

	  if (tstamp <= twindow_end)
	    {
	      /* Mark the current event as having missed
	       * some (duplicate?) data.
	       */
	      result._flags |=
		EXT_FILE_MERGE_NEXT_SRCID_WITHIN_WINDOW;
	      /* Also mark the item that it should possibly
	       * have been merged with the previous.
	       */
	      item->_flags |=
		EXT_FILE_MERGE_PREV_SRCID_WITHIN_WINDOW;
	    }

	  if (tstamp < toldest_cur)
	    toldest_cur = tstamp;
	}
    }

  MRG_DBG ("merge %016llx...  %d chunks\n",
	   toldest, _num_merge_incl);

  assert(_num_merge_incl >= 1);

  /* We have now found all the items to include in the merge. */

  size_t need =
    sizeof (external_writer_buf_header) + incl0_prelen + maxdestplen;

  if (need > _merge_dest_alloc)
    {
      _merge_dest_alloc = need;

      _merge_dest = (uint32_t *)
	realloc (_merge_dest, _merge_dest_alloc);

      if (!_merge_incl)
	ERR_MSG("Failure (re)allocating memory for "
		"merger destination (%zd bytes).", _merge_dest_alloc);
    }

  external_writer_buf_header *header =
    (external_writer_buf_header *) _merge_dest;

  header->_request = htonl(EXTERNAL_WRITER_BUF_NTUPLE_FILL |
			   EXTERNAL_WRITER_REQUEST_HI_MAGIC);
  header->_length = (uint32_t) -1;

  uint32_t *pre = (uint32_t *) (header + 1);

  memcpy(pre, incl0_msg, incl0_prelen);

  uint32_t *pdest = (uint32_t *) (((char *) pre) + incl0_prelen);

  uint32_t *pend = ext_merge_do_merge(oa, pdest, &result);

  uint32_t length = (uint32_t) ((char *) pend - (char *) _merge_dest);

  header->_length = htonl(length);

  MRG_DBG ("merged: %x %zd\n", result._flags, pend - _merge_dest);

  for (uint32_t *p = _merge_dest; p < pend; p++)
    {
      MRG_DBG ("%08x\n", *p);
    }

  uint32_t left = length - sizeof (external_writer_buf_header);

  request_ntuple_fill(comm,
		      pre, &left,
		      header, length,
		      true);

  assert(left == 0);

  MRG_DBG ("-------------------------------------------------------\n");

  return true;
}

void ext_merge_sort_all_until(ext_write_config_comm *comm,
			      offset_array *oa,
			      uint64_t t_until,
			      uint32_t maxdestplen)
{
  for ( ; ; )
    {
      if (!ext_merge_sort_until(comm, oa, t_until, maxdestplen))
	break;
    }
}

void ext_merge_insert_chunk(ext_write_config_comm *comm,
			    offset_array *oa,
			    uint32_t *msgstart,
			    uint32_t prelen, uint32_t plen,
			    uint32_t maxdestplen)
{
  uint32_t meventno = 0;
  uint32_t srcid;

  uint32_t tstamp_hi;
  uint32_t tstamp_lo;

  uint32_t *pstart = (uint32_t *) (((char *) msgstart) + prelen);
  uint32_t *pend = (uint32_t *) (((char *) pstart) + plen);

  for (uint32_t *p = msgstart; p < pend; p++)
    {
      MRG_DBG ("%08x\n", *p);
    }

  tstamp_lo = ntohl(pstart[oa->_poffset_ts_lo]);
  tstamp_hi = ntohl(pstart[oa->_poffset_ts_hi]);
  srcid     = ntohl(pstart[oa->_poffset_ts_srcid]);

  if (oa->_poffset_meventno != (uint32_t) -1)
    meventno  = ntohl(pstart[oa->_poffset_meventno]);

  uint64_t tstamp =
    (((uint64_t) tstamp_hi) << 32) + tstamp_lo;

  /* When we receive data for a chunk with meventno == 0, then
   * we know that we will not get any further data with smaller
   * timestamps.  We can therefore sort the data for all
   * times up to the given time.
   */

  if (meventno == 0)
    {

      ext_merge_sort_until(comm, oa,
			   tstamp - _config._ts_merge_window,
			   maxdestplen);


    }

  MRG_DBG("merge_insert: srcid:%d ts:%08x:%08x meventno:%d (%d+%d)\n",
	  srcid, tstamp_hi, tstamp_lo, meventno, prelen, plen);

  if (srcid >= _alloc_items_store)
    {
      size_t prev_num = _alloc_items_store;

      if (!_alloc_items_store)
	_alloc_items_store = 16;
      while (srcid >= _alloc_items_store)
	_alloc_items_store *= 2;

      _items_store = (merge_item_store **)
	realloc (_items_store,
		 _alloc_items_store * sizeof (merge_item_store *));

      if (!_items_store)
	ERR_MSG("Failure (re)allocating memory for "
		"merger reorder store info (%zd items).", _alloc_items_store);

      for ( ; prev_num < _alloc_items_store; prev_num++)
	_items_store[prev_num] = NULL;

      /* We also reallocate the array of items to include in each merge.
       * When we are here, they are not used anyhow.
       */

      _merge_incl = (merge_item_incl *)
	realloc (_merge_incl, _alloc_items_store * sizeof (merge_item_incl));

      if (!_merge_incl)
	ERR_MSG("Failure (re)allocating memory for "
		"merger reorder include info (%zd items).", _alloc_items_store);
    }

  merge_item_store *store = _items_store[srcid];

  if (!store)
    {
      store = (merge_item_store *) malloc (sizeof (merge_item_store));

      if (!store)
	ERR_MSG("Failure allocating memory for merger reorder store control.");

      _items_store[srcid] = store;

      //store->_events = 0;
      store->_offset_first = 0;
      //store->_offset_last = 0;
      store->_used = 0;
      store->_alloc = 0;
      store->_buf = NULL;
    }
  else if (store->_offset_first >= store->_used)
    {
      /* Reset the store. */
      store->_offset_first = 0;
      store->_used = 0;
    }

  size_t need = store->_used + sizeof (merge_item_chunk) + prelen + plen;

  if (need > store->_alloc)
    {
      if (!store->_alloc)
	store->_alloc = 0x1000;
      while (store->_alloc < need)
	store->_alloc *= 2;

      store->_buf = realloc (store->_buf, store->_alloc);

      if (!store->_buf)
	ERR_MSG("Failure (re)allocating memory for "
		"merger reorder store (%zd bytes).", store->_alloc);
    }

  merge_item_chunk *item =
    (merge_item_chunk *) ((char *) store->_buf + store->_used);

  /* Next item (if present), will be after us? */
  // item->_offset_next = need;
  item->_tstamp_lo = tstamp_lo;
  item->_tstamp_hi = tstamp_hi;
  item->_prelen = prelen;
  item->_plen = plen;
  item->_flags = 0;

  uint32_t *data = (uint32_t *) (item + 1);

  memcpy (data, msgstart, prelen + plen);

  /*
  if (store->_offset_last < store->_used)
    {
      merge_item_chunk *item_last =
	(merge_item_chunk *) ((char *) store->_buf + store->_offset_last);

      item_last->_offset_next = store->_used;
    }
  */

  /* Update the pointer to the last item, and the total amount used. */
  //store->_offset_last = store->_used;
  store->_used = need;
}

void ext_merge_sort_all(offset_array *oa,
			uint32_t maxdestplen)
{
  ext_merge_sort_all_until(NULL /* no comm */,
			   oa,
			   (uint64_t) -1,
			   maxdestplen);
}



/*
  file_input/empty_file --lmd --random-trig --caen-v775=2 --caen-v1290=2 \
    --wr-stamp=mergetest --events=30 > \
    file30.lmd

  xtst/xtst file30.lmd \
    --ntuple=UNPACK,regress1,ID=xtst_regress,STRUCT,gdb,- > \
    ext30_many.dump

  hbook/struct_writer - --header=ext30_many.hh < ext30_many.dump

  hbook/struct_writer - --time-stitch=1000 < ext30_many.dump | less
*/

