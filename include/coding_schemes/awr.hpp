#ifndef CODING_SCHEMES_AWR_HPP_ 
#define CODING_SCHEMES_AWR_HPP_ 

#include "coding_scheme.hpp"

namespace pommel {

class awr: public coding_scheme {
    protected:
        unsigned int N;
    public:
        awr(platform_config_t platform, unsigned int N);
        void encoder(std::istream &in, std::ostream &out);
        void decoder(std::istream &in, std::ostream &out);
};

}

#endif
