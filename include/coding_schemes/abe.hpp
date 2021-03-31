#ifndef CODING_SCHEMES_ABE_HPP_ 
#define CODING_SCHEMES_ABE_HPP_ 

#include "coding_scheme.hpp"

namespace pommel {

class abe: public coding_scheme {
    protected:
        unsigned int channels;    
        unsigned int window_size;    

    public:
        abe(platform_config_t platform, unsigned int window_size);
        void encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out);
};

}

#endif
