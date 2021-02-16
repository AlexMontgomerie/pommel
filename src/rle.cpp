#include "coding_schemes/rle.hpp"

namespace pommel {

void rle::rle_encoder(std::istream &in, std::ostream &out) { 

    int zero_counter = 0;
    int zero_counter_max = ((1<<platform.bitwidth)-1);

    // value buffer
    uint32_t val;

    // iterate over stream
    while(in >> val) {
        //printf("%d\n", zero_counter);
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

void rle::encoder(std::istream &in, std::ostream &out) { 

    // setup string streams
    std::stringstream rle_in;
    std::stringstream rle_out;

    // de-interleave stream in
    deinterleave(in, rle_in); 

    // perform run length encoding
    rle_encoder(rle_in, rle_out);

    // interleave stream again
    interleave(rle_out, out); 

}

rle::rle(platform_config_t platform, unsigned int rle_zero) : coding_scheme(platform), rle_zero(rle_zero){}

}
