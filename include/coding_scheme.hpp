#ifndef CODING_SCHEME_HPP_ 
#define CODING_SCHEME_HPP_ 

#include "common.hpp"

class coding_scheme {
    protected:
        unsigned int bitwidth;
        void correlator(std::istream &in, std::ostream &out);
        void decorrelator(std::istream &in, std::ostream &out);
        
    public:
        coding_scheme(unsigned int bitwidth);

};

#endif
