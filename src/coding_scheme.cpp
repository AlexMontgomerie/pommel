#include "coding_scheme.hpp"

namespace pommel {

//uint32_t coding_scheme::hamming_distance(auto a, auto b) { 
//    return __builtin_popcount(a ^ b); 
//}

void coding_scheme::decorrelator(std::istream &in, std::ostream &out) {

    // value buffer
    std::string line;
    uint128_t val;

    // queue (depth = channels)
    std::queue<uint128_t> fifo;

    // fill buffer, and send first value
    std::getline(in, line);
    val = convert_to_uint128(line);
    fifo.push(val);
    out << convert_from_uint128(val) << std::endl;

    // iterate over the rest of the stream
    while( std::getline(in, line) ) {
        val = convert_to_uint128(line);
        // get delayed value
        uint128_t val_delay = fifo.front();
        fifo.pop();
        uint128_t val_out = val^val_delay;
        // fill buffer
        fifo.push(val_out);
        // write difference to output
        out << convert_from_uint128(val_out) << std::endl;
    }
}

void coding_scheme::interleave(std::istream &in, std::ostream &out, int bitwidth, int packing_factor) {

    // mask
    uint32_t mask = ( 1 << bitwidth ) - 1;

    // interleave 
    uint32_t val;
    uint128_t val_out = 0;
    int i = 0;
    while(in >> val) {
        val_out |=( (uint64_t) ( val & mask ) ) << i*bitwidth;
        i = ( i + 1 ) % packing_factor;
        if( i == 0 ) {
            out << convert_from_uint128(val_out) << std::endl;
            val_out = 0;
        }
    }
}

void coding_scheme::deinterleave(std::istream &in, std::ostream &out, int bitwidth, int packing_factor) {

    // mask
    uint32_t mask = ( 1 << bitwidth ) - 1;

    // interleave 
    uint128_t val;
    std::string line;
    uint32_t val_out = 0;
    while( std::getline(in, line) ) {
        val = convert_to_uint128(line);
        for(int i=0;i<packing_factor;i++) {
            val_out = (val >> i*bitwidth) & mask;
            out << val_out << std::endl;
        }
    }
}

coding_scheme::coding_scheme(platform_config_t platform) : platform(platform){}

}
