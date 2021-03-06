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

#include "sst_ext.hh"

#include "error.hh"

#include "unfinished/huffman.c"

/*---------------------------------------------------------------------------*/

#include "structures.hh"

/*---------------------------------------------------------------------------*/

#include "gen/sst_ped_add.hh"

int EXT_SST_initialized = 0;

EXT_SST::EXT_SST()
{
  if (!EXT_SST_initialized)
    {
      setup_huffman();
      setup_unpack_tables();
      init_sst_ped_add();
      EXT_SST_initialized = 1;
    }
}

EXT_SST::~EXT_SST()
{
}

void EXT_SST::__clean()
{
  header.u32 = 0;
  //for (int i = 0; i < countof(data); i++)
  //  data[i].__clean();
  data.__clean();
}

union EXT_SST_ch_data
{
  struct
  {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint32 value : 12; // 0..11
    uint32 adc_no : 2; // 12..13
    uint32 dummy_14_15 : 2;
    uint32 channel : 9; // 16..24
    uint32 dummy_25_27 : 3;
    uint32 unnamed_28_31 : 4; // 28..31
#endif
#if __BYTE_ORDER == __BIG_ENDIAN
    uint32 unnamed_28_31 : 4; // 28..31
    uint32 dummy_25_27 : 3;
    uint32 channel : 9; // 16..24
    uint32 dummy_14_15 : 2;
    uint32 adc_no : 2; // 12..13
    uint32 value : 12; // 0..11
#endif
  };
  uint32  u32;
};

EXT_DECL_DATA_SRC_FCN_ARG(void,EXT_SST::__unpack,uint32 sam,uint32 gtb,uint32 siderem,uint32 branch)
{
  GET_BUFFER_UINT32(header.u32);

  // We've already checked that sam, gtb and siderem are correct.
  // The local_trigger and local_event_counter we can not validate.

  // The count will be used while unpacking...

  if (!header.count)
    return; // data is old-style, and has no content

  // There has to be at least count words left of data in the buffer

  if (__buffer.left() < sizeof(uint32) * header.count)
    ERROR("Count (%ddw = %d bytes) larger than data space left (%d).",
	  header.count,(int) (sizeof(uint32) * header.count),
	  (int) __buffer.left());

  // Next we need to know if it is old style data, marked by 0xf in the
  // high 4 bits

  uint32 first_word = 0; // init=0 to keep compiler quiet

  __buffer.peek_uint32(&first_word);

  // printf ("%d %d %d : %4d : %08x\n",sam,gtb,siderem,header.count,first_word);

  if ((first_word & 0xf0000000) == 0xf0000000)
    {
      // Old style

      // Unpack one channel at a time.  They are known to be in order,
      // check that.

      int last_index = -1;

      for (int i = header.count; i; --i)
	{
	  static uint32 max_adc_channel[4] = { 320, 320, 384, 0 };

	  EXT_SST_ch_data ch_data;

	  __buffer.get_uint32(&ch_data.u32);

	  if ((ch_data.u32 & 0xfe00c000) != 0xf0000000)
	    ERROR("SST data fixed bits error (0x%08x & 0x%08x) != 0x%08x",
		  ch_data.u32,0xfe00c000,0xf0000000);

	  if (ch_data.channel > max_adc_channel[ch_data.adc_no])
	    ERROR("SST data channel (%d) (max %d) to high for adc %d",
		  ch_data.channel,max_adc_channel[ch_data.adc_no],ch_data.adc_no);

	  int index = (ch_data.adc_no * 320) + ch_data.channel;

	  if (index <= last_index)
	    ERROR("SST data unordered (this %d <= prev %d)",index,last_index);

	  DATA12 &item = data.insert_index(-1,index);

	  item.value = ch_data.value;
	}

      return;
    }

  __buffer.advance(sizeof(uint32)); // take first_word as eaten

  uint32 huff_words = header.count-2;

  // New style (huffman encoded...)

  uint32 coffee_info = 0;
  uint32 coffee_version = 0;

  if (first_word == 0x01c0ffee)
    {
      __buffer.get_uint32(&coffee_version);
      // printf ("version %08x\n", coffee_version);
      __buffer.get_uint32(&coffee_info);

      huff_words -= 2;
    }
  else if (first_word != 0)
    ERROR("Unexpected SST subheader: %08x",first_word);

  // The count should be at least 2 (first word, and the huffman XOR)

  if (header.count < 2)
    ERROR("Word count (%d) for SST huffman encoded < 2",header.count);

  // Now try to huffman decode the stuff...

  uint32 dest[1024];
  uint32 *dest_end = dest + 1024;

  uint32 src[1024];
  uint32 *src_end = src;
  uint32 encoded_xor_calc = 0;

  if (first_word == 0x01c0ffee)
    {
      encoded_xor_calc ^= first_word;
      encoded_xor_calc ^= coffee_version;
      encoded_xor_calc ^= coffee_info;
    }

  for (int i = huff_words; i; --i)
    {
      __buffer.get_uint32(src_end);
      encoded_xor_calc ^= *src_end;

      // printf ("%d %08x %08x\n",i,*src_end, encoded_xor_calc);

      src_end++;
    }

  uint32 encoded_xor = 0; // init=0 to keep compiler quiet

  __buffer.get_uint32(&encoded_xor);

  if (encoded_xor_calc != encoded_xor)
    ERROR("SST XOR data mismatch (calc %08x != store %08x, %08x) "
	  "sam%dgtb%dsid%d",
	  encoded_xor_calc,encoded_xor,
	  encoded_xor_calc ^ encoded_xor,
	  sam,gtb,siderem);

  if (first_word == 0x01c0ffee)
    {
      /* 1028 was not correct as worst-case length.
       * In case the packaging has jumped over single values,
       * then there will be an additional chunk, which uses two values
       * to pack.  Packaging should have merged these as well, as
       * that would be cheaper.  Anyhow, no major harm.
       */
      uint32 chunk[2048];
      // forgot XOR

      uint32 huff_len = coffee_info & 0xffff;
      if (huff_len > countof(chunk))
        {
          ERROR("SST chunk data length %u > max %u "
                "sam%dgtb%dsid%d",
                huff_len, (uint32)countof(chunk),
                sam,gtb,siderem);
          return;
        }
      uint32 chunks = coffee_info >> 16;

      uint32 *chunk_end = chunk + huff_len;

#if 0
      /* Hard-code an event: */

      src_end = src;

      huff_len = 0x1c;
      uint32_t chunk_end = chunk + huff_len;

      *(src_end++) = 0xbbed93df;
      *(src_end++) = 0xc000678d;
      *(src_end++) = 0x0000000d;
      *(src_end++) = 0x10001dc0;
#endif

      //printf ("decode and get %d words.\n", huff_len);

      decode_stream(src,src_end,
		    chunk,chunk_end);

      uint32 *pstarts = chunk;
      uint32 *plens   = chunk + chunks;
      uint32 *pvalues = chunk + chunks * 2;

      uint32 prev_start = 0;

      uint32 *dp = dest;
      uint32 j = 0;
      for (uint32 i = 0; i < chunks; i++)
	{
	  uint32 start, len, end;

	  start = *(pstarts++) + prev_start;
	  len   = *(plens++);
          end   = start + len;

	  //printf ("[%d..%d]:\n", start, end-1);
	  /*
	  if (huff_len > 1028)
	    printf ("[%d..%d]:\n", start, end-1);
	  */

          for (; j < start; j++)
            {
              *dp++ = 0;
            }
	  for (; j < end; j++)
	    {
	      //printf (" %4d: %4d\n", j, *(pvalues++));
              *dp++ = *pvalues++;
	    }

	  prev_start = start;
	}

      if (pvalues != chunk_end)
	ERROR("Wrong total number of values.");

      for (; j < 1024; j++)
        {
          *dp++ = 0;
        }

      assert(dp == dest_end);
    }
  else
    {
      decode_stream(src,src_end,
          dest,dest_end);
    }

  /*
  UNUSED(src);
  UNUSED(src_end);
  UNUSED(dest);
  UNUSED(dest_end);
  */

  uint32* ped_add = _sst_ped_add[branch][sam][gtb][siderem];

  if (!ped_add)
    ERROR("(compression) pedestals for branch%d sam%dgtb%dsid%d not known",
	  branch,sam,gtb,siderem);

  // Since we know that we have 1024 data words, we'll just copy them
  // in by force :-)

  memset(data._valid._bits,0xff,sizeof(data._valid._bits));
  for (int i = 0; i < 1024; i++)
    data._items[i].value = (dest[i] + ped_add[i]) & 0x0fff;

  return;

  // Since we have checked availability, we may use
  // __buffer.get_uint32(&dest) and __buffer.peek_uint32(&dest)
}
EXT_FORCE_IMPL_DATA_SRC_FCN_ARG(void,EXT_SST::__unpack,uint32 sam,uint32 gtb,uint32 siderem,uint32 branch)

EXT_DECL_DATA_SRC_FCN_ARG(void,EXT_SST::__unpack,uint32 sam,uint32 gtb,uint32 siderem)
{
  __unpack(__buffer,sam,gtb,siderem,0);
}
EXT_FORCE_IMPL_DATA_SRC_FCN_ARG(void,EXT_SST::__unpack,uint32 sam,uint32 gtb,uint32 siderem)


EXT_DECL_DATA_SRC_FCN_ARG(bool,EXT_SST::__match,uint32 sam,uint32 gtb,uint32 siderem,uint32 branch)
{
  /* TODO: using an external unpacker like this prevents the optimized
   * ucesb finder from using the changed bit-pattern between used
   * detectors and invoking a lookup-table...
   */

  UNUSED(branch);

  EXT_SST_header header_tmp;

  GET_BUFFER_UINT32(header_tmp.u32);

  if (header_tmp.sam == sam &&
      header_tmp.gtb == gtb &&
      header_tmp.siderem == siderem)
    return true;

  return false;
}
EXT_FORCE_IMPL_DATA_SRC_FCN_ARG(bool,EXT_SST::__match,uint32 sam,uint32 gtb,uint32 siderem,uint32 branch)

EXT_DECL_DATA_SRC_FCN_ARG(bool,EXT_SST::__match,uint32 sam,uint32 gtb,uint32 siderem)
{
  return __match(__buffer,sam,gtb,siderem,0);
}
EXT_FORCE_IMPL_DATA_SRC_FCN_ARG(bool,EXT_SST::__match,uint32 sam,uint32 gtb,uint32 siderem)


/*---------------------------------------------------------------------------*/
