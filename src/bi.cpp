#include "coding_schemes/bi.hpp"

namespace pommel {

void bi::encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out) {

    // send addresses in to addresses out
    addr_out << addr_in.rdbuf();

    // effective bus width
    uint32_t bus_width = platform.bitwidth*platform.packing_factor;
    
    // mask
    uint128_t mask = (1 << bus_width) - 1;

    // value buffer
    std::string line;
    uint128_t val;

    // queue (depth = 1)
    std::queue<uint128_t> fifo;

    // fill buffer, and send first value
    std::getline(data_in, line);
    val = convert_to_uint128(line);
    
    fifo.push(val);
    data_out << convert_from_uint128(val) << std::endl;

    // iterate over the rest of the stream
    while ( std::getline(data_in, line) ) {

        // convert line to string stream
        val = convert_to_uint128(line);

        // get delayed value
        uint128_t val_delay = fifo.front();
        fifo.pop();
        bool invert = false;
        // check hamming distance
        if (__builtin_popcount(val ^ val_delay) > bus_width / 2) {
            invert = true;
            val = ~val & mask;
        }
        // add side channel signal
        if (invert) {
            val |= 1 << bus_width;
        }
        fifo.push(val);
        data_out << convert_from_uint128(val) << std::endl;
    }
}

bi::bi(platform_config_t platform) : coding_scheme(platform){}

}
