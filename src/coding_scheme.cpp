#include "coding_scheme.hpp"

namespace pommel {

uint32_t coding_scheme::hamming_distance(uint32_t a, uint32_t b) { 
    return __builtin_popcount(a ^ b); 
}

void coding_scheme::decorrelator(std::istream &in, std::ostream &out) {

    // value buffer
    uint64_t val;

    // queue (depth = channels)
    std::queue<uint64_t> fifo;

    // fill buffer, and send first value
    in >> val;
    fifo.push(val);
    out << val << std::endl;

    // iterate over the rest of the stream
    while(in >> val) {
        // get delayed value
        uint64_t val_delay = fifo.front();
        fifo.pop();
        uint64_t val_out = val^val_delay;
        // fill buffer
        fifo.push(val_out);
        // write difference to output
        out << val_out << std::endl;
    }
}

void coding_scheme::interleave(std::istream &in, std::ostream &out, int bitwidth, int packing_factor) {

    // mask
    uint32_t mask = ( 1 << bitwidth ) - 1;

    // interleave 
    uint32_t val;
    uint64_t val_out = 0;
    int i = 0;
    while(in >> val) {
        val_out |=( (uint64_t) ( val & mask ) ) << i*bitwidth;
        i = ( i + 1 ) % packing_factor;
        if( i == 0 ) {
            out << val_out << std::endl;
            val_out = 0;
        }
    }
}

void coding_scheme::deinterleave(std::istream &in, std::ostream &out, int bitwidth, int packing_factor) {

    // mask
    uint32_t mask = ( 1 << bitwidth ) - 1;

    // interleave 
    uint64_t val;
    uint32_t val_out = 0;
    while(in >> val) {
        for(int i=0;i<packing_factor;i++) {
            val_out = (val >> i*bitwidth) & mask;
            out << val_out << std::endl;
        }
    }
}

coding_scheme::coding_scheme(platform_config_t platform) : platform(platform){}

}
