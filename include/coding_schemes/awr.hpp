#ifndef CODING_SCHEMES_AWR_HPP_ 
#define CODING_SCHEMES_AWR_HPP_ 

#include "coding_scheme.hpp"

namespace pommel {

class awr: public coding_scheme {
    protected:
        unsigned int N;
    public:
        awr(platform_config_t platform, unsigned int N);
        void encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out);
};

}

#endif
