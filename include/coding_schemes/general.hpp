#ifndef CODING_SCHEMES_GENERAL_HPP_
#define CODING_SCHEMES_GENERAL_HPP_

#include "coding_scheme.hpp"

namespace pommel {

class general: public coding_scheme {
    protected:
        float compression_ratio;
        float activity;
        unsigned int batch_size;
    public:
        general(platform_config_t platform, float compression_ratio, float activity, unsigned int batch_size);
        void encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out);
};

}

#endif
