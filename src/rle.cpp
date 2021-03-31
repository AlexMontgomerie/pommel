#include "coding_schemes/rle.hpp"

namespace pommel {

void rle::rle_encoder(std::istream &in, std::ostream &out) { 

    int zero_counter = 0;
    int zero_counter_max = ((1<<platform.bitwidth)-1);

    // get bit mask
    uint32_t mask = (1 << platform.bitwidth) - 1;

    // value buffer
    uint32_t val;

    // iterate over stream
    while(in >> val) {
        // put through mask
        val &= mask;
        if( val != rle_zero ) {
            if( zero_counter == 0 ) {
                out << val << std::endl;
            } else {
                while( zero_counter > zero_counter_max ) {
                    out << rle_zero << std::endl;
                    out << zero_counter_max << std::endl;
                    zero_counter -= zero_counter_max;
                }
                out << rle_zero << std::endl;
                out << zero_counter << std::endl;
                out << val << std::endl;
                zero_counter = 0;
            }
        } else {
            zero_counter++;
        }
    }
    
    // send last zeros out
    if( zero_counter != 0 ) {
        while( zero_counter > zero_counter_max ) {
            out << rle_zero << std::endl;
            out << zero_counter_max << std::endl;
            zero_counter -= zero_counter_max;
        }
        out << rle_zero << std::endl;
        out << zero_counter << std::endl;
    }
}

void rle::encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out) {

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
        std::stringstream rle_in;
        std::stringstream rle_out;

        // de-interleave stream in
        deinterleave(data_buffer, rle_in, platform.bitwidth, platform.packing_factor); 

        // perform run length encoding
        rle_encoder(rle_in, rle_out);

        // interleave stream again
        interleave(rle_out, encoded_data_buffer, platform.bitwidth, platform.packing_factor); 

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

rle::rle(platform_config_t platform, unsigned int rle_zero, unsigned int batch_size) : coding_scheme(platform), rle_zero(rle_zero), batch_size(batch_size){}

}
