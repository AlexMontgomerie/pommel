#ifndef CODING_SCHEME_HPP_ 
#define CODING_SCHEME_HPP_ 

#include "common.hpp"
#include "config.hpp"

namespace pommel {

class coding_scheme {
    protected:

        platform_config_t platform;

        //uint32_t hamming_distance(auto a, uint128_t b); 

        void correlator(std::istream &in, std::ostream &out);
        void decorrelator(std::istream &in, std::ostream &out);
        
        void interleave(std::istream &in, std::ostream &out, int bitwidth, int packing_factor);
        void deinterleave(std::istream &in, std::ostream &out, int bitwidth, int packing_factor);

    public:

        coding_scheme(platform_config_t platform);

        void encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out);
        //void decoder(std::istream &data_in, std::ostream &out);

};

}

#endif
