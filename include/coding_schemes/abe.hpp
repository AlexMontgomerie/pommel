#ifndef CODING_SCHEMES_ABE_HPP_ 
#define CODING_SCHEMES_ABE_HPP_ 

#include "coding_scheme.hpp"

class abe: public coding_scheme {
    protected:
        unsigned int channels;    
        unsigned int window_size;    

    public:
        abe(unsigned int bitwidth, unsigned int window_size);
        void encoder(std::istream &in, std::ostream &out);
        void decoder(std::istream &in, std::ostream &out);
};

#endif
