#include "coding_scheme.hpp"

uint32_t coding_scheme::hamming_distance(uint32_t a, uint32_t b) { 
    return __builtin_popcount(a ^ b); 
}

void coding_scheme::decorrelator(std::istream &in, std::ostream &out) {

    // mask
    uint32_t mask = (1<<bitwidth)-1;

    // value buffer
    uint32_t val;

    // queue (depth = channels)
    std::queue<uint32_t> fifo;

    // fill buffer, and send first value
    in >> val;
    fifo.push(val);
    out << val << std::endl;

    // iterate over the rest of the stream
    while(in >> val) {
        // get delayed value
        uint32_t val_delay = fifo.front();
        fifo.pop();
        uint32_t val_out = val^val_delay;
        // fill buffer
        fifo.push(val_out);
        // write difference to output
        out << val_out << std::endl;
    }
}

coding_scheme::coding_scheme(unsigned int bitwidth) : bitwidth(bitwidth){}
