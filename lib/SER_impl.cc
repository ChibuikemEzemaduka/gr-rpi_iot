/* -*- c++ -*- */
/*
 * Copyright 2019 Yu Wang.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <memory>
#include "SER_impl.h"


namespace gr {
  namespace rpi_iot {

    SER::sptr
    SER::make(long window_size)
    {
      return std::make_shared<SER_impl>(window_size);        
    }

    /*
     * The private constructor
     */
    SER_impl::SER_impl(long window_size)
      : gr::block("SER",
              gr::io_signature::make(2, 2, sizeof(char)),
              gr::io_signature::make(1, 1, sizeof(float))
            ),
            cur_store_idx(0)
    {
      if (window_size > 0){
        d_window_size = window_size;
      }
      else{
        std::cout<<"window_size should be greater than 0. Reset to 100000\n";
        d_window_size = 100000;
      }
      // allocate the size
      comp_stack.assign(d_window_size, 0);

      // std::cout<<"SER initialization finished!\n";
    }

    /*
     * Our virtual destructor.
     */
    SER_impl::~SER_impl()
    {
    }

    void
    SER_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      unsigned long ninputs = ninput_items_required.size ();
      for(unsigned long i = 0; i < ninputs; i++)
        ninput_items_required[i] = noutput_items;
    }

    int
    SER_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in0 = (const unsigned char *) input_items[0];
      const unsigned char *in1 = (const unsigned char *) input_items[1];
      float *out = (float *) output_items[0];
      float SER = 0;
      long num_of_errors = 0;

      noutput_items = ninput_items[0]<ninput_items[1]?ninput_items[0]:ninput_items[1];

      // std::cout<<int(in0[0])<<"  "<<int(in1[0])<<std::endl;

      for (int idx = 0; idx < noutput_items; idx++){
        if (in0[idx] != in1[idx]){
          comp_stack[cur_store_idx] = 1;
        }
        else{
          comp_stack[cur_store_idx] = 0;
        }
        cur_store_idx = (cur_store_idx + 1) % d_window_size;
      }

      // compute how many errors in the comparison stack
      for(long idx = 0; idx < d_window_size; idx++){
        if (comp_stack[idx])
          num_of_errors ++;
      }

      // update the SER
      SER = num_of_errors / double(d_window_size);
      for (int idx = 0; idx < noutput_items; idx++){
        out[idx] = SER;
      }


      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace rpi_iot */
} /* namespace gr */
