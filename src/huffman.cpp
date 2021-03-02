#include "coding_schemes/huffman.hpp"

namespace pommel {

void huffman::huffman_bitstream_encoder(std::istream &in, std::ostream &out) { 

    // value buffer
    uint32_t val;

    // iterate over the rest of the stream
    while (in >> val) {
       
        // convert from input to code 
        uint32_t encoded_val = code_table[val];
   

        // convert this code to a single bitstream
        bool reached_first_one = false;
        for(int i = 0; i < 32; i++) {

            // get bit value
            uint32_t bit = ( encoded_val >> (31-i) ) & 1;
            
            // ignore leading zeros
            if( (!reached_first_one) && (bit == 1) ) {
                reached_first_one = true;
            }
            
            // send single bit
            if(reached_first_one) {
                out << bit << std::endl;
            }
        }
    }

}

void huffman::encoder(std::istream &in, std::ostream &out) { 

    // setup string streams
    std::stringstream huffman_in;
    std::stringstream huffman_out;

    // de-interleave stream in
    deinterleave(in, huffman_in, platform.bitwidth, platform.packing_factor); 

    // perform run length encoding
    huffman_bitstream_encoder(huffman_in, huffman_out);

    // interleave stream again
    interleave(huffman_out, out, 1, platform.packing_factor*platform.bitwidth); 
    //interleave(huffman_out, out, platform.bitwidth, platform.packing_factor); 

}

huffman::huffman(platform_config_t platform, std::map<uint32_t,uint32_t> code_table) : coding_scheme(platform), code_table(code_table) {}

}
