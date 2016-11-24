
#include "monitor.hh"
#include "optimise.hh"

#include <string.h>
#include <stdio.h>

status_block _status_block;

void mon_block_update(status_block *handle, status_monitor *data)
{
  int copy;

  copy = (++handle->_copy_seq_done) & 1;

  /* Update the marker that we are now updating this copy.
   * (marker [0] does not match marker [1] during the copy.
   */

  handle->_copy_seq[copy][0] = handle->_copy_seq_done;

  SFENCE;

  memcpy (&handle->_copy[copy], data, sizeof (handle->_copy[copy]));

  SFENCE;

  /* Update is done, let marker[1] match. */

  handle->_copy_seq[copy][1] = handle->_copy_seq_done;

  printf ("Copied status block to %d\n", copy);
}

void monitor_thread()
{



}
