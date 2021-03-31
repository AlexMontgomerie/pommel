#ifndef CODING_SCHEMES_DEF_HPP_ 
#define CODING_SCHEMES_DEF_HPP_ 

#include "coding_scheme.hpp"

namespace pommel {

class def: public coding_scheme {
    protected:
        unsigned int channels;    
        int int_to_smint(int in);
        int smint_to_int(int in);
        void diff_encoder(std::istream &in, std::ostream &out);

    public:
        def(platform_config_t platform, unsigned int channels);
        void encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out);
};

}

#endif
