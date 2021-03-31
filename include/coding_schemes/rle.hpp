#ifndef CODING_SCHEMES_RLE_HPP_ 
#define CODING_SCHEMES_RLE_HPP_ 

#include "coding_scheme.hpp"

namespace pommel {

class rle: public coding_scheme {
    protected:
        unsigned int rle_zero;
        unsigned int batch_size;
        void rle_encoder(std::istream &in, std::ostream &out);
    public:
        rle(platform_config_t platform, unsigned int rle_zero, unsigned int batch_size);
        void encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out);
};

}

#endif
