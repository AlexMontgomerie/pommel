#include "coding_schemes/bi.hpp"

namespace pommel {

void bi::encoder(std::istream &in, std::ostream &out) {

    // effective bus width
    uint32_t bus_width = platform.bitwidth*platform.packing_factor;
    
    // mask
    uint64_t mask = (1 << bus_width) - 1;

    // value buffer
    uint64_t val;

    // queue (depth = 1)
    std::queue<uint64_t> fifo;

    // fill buffer, and send first value
    in >> val;
    fifo.push(val);
    out << val << std::endl;

    // iterate over the rest of the stream
    while (in >> val) {
        // get delayed value
        uint64_t val_delay = fifo.front();
        fifo.pop();
        bool invert = false;
        // check hamming distance
        if (hamming_distance(val, val_delay) > bus_width / 2) {
            invert = true;
            val = ~val & mask;
        }
        // add side channel signal
        if (invert) {
            val |= 1 << bus_width;
        }
        fifo.push(val);
        out << val << std::endl;
    }
}

bi::bi(platform_config_t platform) : coding_scheme(platform){}

}
