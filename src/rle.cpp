#include "coding_schemes/rle.hpp"

void rle::encoder(std::istream &in, std::ostream &out) { 

    int zero_counter = 0;
    int zero_counter_max = ((1<<bitwidth)-1);

    // value buffer
    uint32_t val;

    // iterate over the rest of the stream
    while(in >> val) {
        
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


rle::rle(unsigned int bitwidth, unsigned int rle_zero) : coding_scheme(bitwidth), rle_zero(rle_zero){}
