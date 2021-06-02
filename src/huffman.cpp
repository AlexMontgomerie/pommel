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

void huffman::encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out) {

    while( data_in.rdbuf()->in_avail() ) {
        // create buffer
        std::stringstream data_buffer;
        std::stringstream addr_buffer;
        std::string data_line;
        std::string addr_line;
        for(int i=0; i<batch_size;i++) {
            // read in streams
            std::getline(data_in, data_line);
            std::getline(addr_in, addr_line);
            // check if empty
            if( data_line.empty() || addr_line.empty() )
                break;
            // write to buffer
            data_buffer << data_line << std::endl;
            addr_buffer << addr_line << std::endl;
        }
        // encode buffer
        std::stringstream encoded_data_buffer;

        // setup string streams
        std::stringstream huffman_in;
        std::stringstream huffman_out;

        // de-interleave stream in
        deinterleave(data_buffer, huffman_in, platform.bitwidth, platform.packing_factor);

        // perform run length encoding
        huffman_bitstream_encoder(huffman_in, huffman_out);

        // interleave stream again
        interleave(huffman_out, encoded_data_buffer, 1, platform.packing_factor*platform.bitwidth);

        // send to output
        std::string addr_line_prev;
        while( std::getline(encoded_data_buffer, data_line) ) {
            // read address buffer also
            std::getline(addr_buffer, addr_line);
            if( addr_line.empty() ) {
                addr_line = addr_line_prev;
            }
            // write to output streams
            addr_line_prev = addr_line;
            data_out << data_line << std::endl;
            addr_out << addr_line << std::endl;
        }
    }
}

huffman::huffman(platform_config_t platform, std::map<uint32_t,uint32_t> code_table, unsigned int batch_size) : coding_scheme(platform), code_table(code_table), batch_size(batch_size) {}

}
