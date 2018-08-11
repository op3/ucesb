/* This file is part of UCESB - a tool for data unpacking and processing.
 *
 * Copyright (C) 2018  Haakan T. Johansson  <f96hajo@chalmers.se>
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

#include "external_writer.hh"

#include <arpa/inet.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#ifndef EXT_STRUCT_CTRL
# define EXT_STRUCT_CTRL(x)
#endif

external_writer *ew;

struct mystruct
{
  int32_t a;
  float c;
  int32_t b;
  float d[4 EXT_STRUCT_CTRL(b)];
  float e[7];
};

mystruct event;

/* The code for the below can be obtained from:
 *
 * ../make_external_struct_sender.pl --struct=mystruct < external_writer_test.cc
 *
 * Which even better would be invoked in the build process.
 * But not in this example.
 */

int main(int argc, char *argv[])
{
  main_argv0 = argv[0];

  ew = new external_writer();

  ew->init(NTUPLE_TYPE_ROOT | NTUPLE_CASE_KEEP, false,
	   "monsterfile.root","Title",-1,false,false,false,false);

  ew->send_file_open();

  ew->send_book_ntuple(101,"h101","CoolTree");

  ew->send_alloc_array(sizeof(mystruct));

  ew->send_hbname_branch("DEF",offsetof(mystruct,a),sizeof(event.a),
			 "a",-1,"",EXTERNAL_WRITER_FLAG_TYPE_INT32);
  ew->send_hbname_branch("DEF",offsetof(mystruct,c),sizeof(event.c),
			 "c",-1,"",EXTERNAL_WRITER_FLAG_TYPE_FLOAT32);
  ew->send_hbname_branch("DEF",offsetof(mystruct,b),sizeof(event.b),
			 "b",-1,"",EXTERNAL_WRITER_FLAG_TYPE_INT32);
  ew->send_hbname_branch("DEF",offsetof(mystruct,d),sizeof(event.d),
			 "d",4,"b",EXTERNAL_WRITER_FLAG_TYPE_FLOAT32);
  ew->send_hbname_branch("DEF",offsetof(mystruct,e),sizeof(event.e),
			 "e",7,"",EXTERNAL_WRITER_FLAG_TYPE_FLOAT32);

  uint32_t offset_msg_size = (1+1+1+(2)+4+7) * (uint32_t) sizeof(uint32_t);
  uint32_t fill_msg_size = (2 + (1+1+1+4+7)) * (uint32_t) sizeof(uint32_t);

  uint32_t maxmsgsize =
    fill_msg_size > offset_msg_size ?
    fill_msg_size : offset_msg_size;

  ew->set_max_message_size(maxmsgsize);

  {
    uint32_t *o = ew->prepare_send_offsets(offset_msg_size);

    *(o++) = htonl((uint32_t) offsetof(mystruct,a) | 0x40000000);
    *(o++) = htonl((uint32_t) offsetof(mystruct,c));
    *(o++) = htonl((uint32_t) offsetof(mystruct,b) | 0x80000000 | 0x40000000);
    *(o++) = htonl(4);
    *(o++) = htonl(1);
    for (int l = 0; l < 4; l++) {
      *(o++) = htonl((uint32_t) ((offsetof(mystruct,d)+l*sizeof(float))));
    }
    for (int i = 0; i < 7; i++)
      *(o++) = htonl((uint32_t) ((offsetof(mystruct,e)+i*sizeof(float))));
    ew->send_offsets_fill(o);
  }

  ew->send_setup_done();

  /* loop over events */
  for (int i = 0; i < 12345; i++)
    {
      uint32_t *p = ew->prepare_send_fill(fill_msg_size);

      *(p++) = htonl(0); /* marker */

      struct mystruct s;

      memset(&s, 0, sizeof (s));

      {
	*(p++) = htonl((s.a));
	*(p++) = htonl(external_write_float_as_uint32(s.c));
	{
	  uint32_t loop_n_b = s.b;
	  assert (loop_n_b <= 4);
	  *(p++) = htonl(loop_n_b);
	  for (int l = 0; l < loop_n_b; l++) {
	    *(p++) = htonl(external_write_float_as_uint32(s.d[l]));
	  }
	}
	for (int i = 0; i < 7; i++)
	  *(p++) = htonl(external_write_float_as_uint32(s.e[i]));
      }     

      ew->send_offsets_fill(p);
    }

  ew->close(); // not required, done in destructor below

  delete ew;
}

#define EXTERNAL_WRITER_NO_SHM 1

#include "external_writer.cc"
#include "forked_child.cc"
#include "markconvbold.cc"
#include "colourtext.cc"
#include "error.cc"
