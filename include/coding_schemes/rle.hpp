#ifndef CODING_SCHEMES_RLE_HPP_ 
#define CODING_SCHEMES_RLE_HPP_ 

#include "coding_scheme.hpp"

class rle: public coding_scheme {
    protected:
        unsigned int rle_zero;
    public:
        rle(unsigned int bitwidth, unsigned int rle_zero);
        void encoder(std::istream &in, std::ostream &out);
        void decoder(std::istream &in, std::ostream &out);
};

#endif
