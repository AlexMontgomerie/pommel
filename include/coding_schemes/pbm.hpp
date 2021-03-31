#ifndef CODING_SCHEMES_PBM_HPP_ 
#define CODING_SCHEMES_PBM_HPP_ 

#include "coding_scheme.hpp"

namespace pommel {

class pbm: public coding_scheme {
    protected:
        std::map<uint32_t,uint32_t> code_table;
        void fv_encoder(std::istream &in, std::ostream &out);
        void fv_decoder(std::istream &in, std::ostream &out);

    public:
        pbm(platform_config_t platform, std::map<uint32_t,uint32_t> code_table);
        void encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out);
};

}

#endif
