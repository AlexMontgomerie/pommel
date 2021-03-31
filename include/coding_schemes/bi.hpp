#ifndef CODING_SCHEMES_BI_HPP_ 
#define CODING_SCHEMES_BI_HPP_ 

#include "coding_scheme.hpp"

namespace pommel {

class bi: public coding_scheme {
    public:
        bi(platform_config_t platform);
        void encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out);
};

}

#endif
