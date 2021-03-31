#ifndef CODING_SCHEMES_HUFFMAN_HPP_ 
#define CODING_SCHEMES_HUFFMAN_HPP_ 

#include "coding_scheme.hpp"

namespace pommel {

class huffman: public coding_scheme {
    protected:
        std::map<uint32_t,uint32_t> code_table;
        unsigned int batch_size;
        void huffman_bitstream_encoder(std::istream &in, std::ostream &out) ;
    public:
        huffman(platform_config_t platform, std::map<uint32_t,uint32_t> code_table, unsigned int batch_size);
        void encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out);
};

}

#endif
