#ifndef CODING_SCHEMES_HUFFMAN_HPP_ 
#define CODING_SCHEMES_HUFFMAN_HPP_ 

#include "coding_scheme.hpp"

namespace pommel {

class huffman: public coding_scheme {
    protected:
        std::map<uint32_t,uint32_t> code_table;
        void huffman_bitstream_encoder(std::istream &in, std::ostream &out) ;
    public:
        huffman(platform_config_t platform, std::map<uint32_t,uint32_t> code_table);
        void encoder(std::istream &in, std::ostream &out);
        void decoder(std::istream &in, std::ostream &out);
};

}

#endif
