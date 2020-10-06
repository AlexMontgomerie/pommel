#ifndef CODING_SCHEME_HPP_ 
#define CODING_SCHEME_HPP_ 

#include "common.hpp"

class coding_scheme {
    protected:
        unsigned int bitwidth;
        uint32_t hamming_distance(uint32_t a, uint32_t b); 
        void correlator(std::istream &in, std::ostream &out);
        void decorrelator(std::istream &in, std::ostream &out);
    public:
        coding_scheme(unsigned int bitwidth);
        void encoder(std::istream &in, std::ostream &out);
        void decoder(std::istream &in, std::ostream &out);
};

#endif
