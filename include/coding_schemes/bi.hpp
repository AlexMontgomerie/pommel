#ifndef CODING_SCHEMES_BI_HPP_ 
#define CODING_SCHEMES_BI_HPP_ 

#include "coding_scheme.hpp"

class bi: public coding_scheme {
    public:
        bi(unsigned int bitwidth);
        void encoder(std::istream &in, std::ostream &out);
        void decoder(std::istream &in, std::ostream &out);
};

#endif
