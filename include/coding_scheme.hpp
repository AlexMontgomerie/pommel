#ifndef CODING_SCHEME_HPP_ 
#define CODING_SCHEME_HPP_ 

#include "common.hpp"
#include "config.hpp"

namespace pommel {

class coding_scheme {
    protected:

        platform_config_t platform;

        uint32_t hamming_distance(uint32_t a, uint32_t b); 

        void correlator(std::istream &in, std::ostream &out);
        void decorrelator(std::istream &in, std::ostream &out);
        
        void interleave(std::istream &in, std::ostream &out);
        void deinterleave(std::istream &in, std::ostream &out);

    public:

        coding_scheme(platform_config_t platform);

        void encoder(std::istream &in, std::ostream &out);
        void decoder(std::istream &in, std::ostream &out);

};

}

#endif
