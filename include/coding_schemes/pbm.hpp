#ifndef CODING_SCHEMES_PBM_HPP_ 
#define CODING_SCHEMES_PBM_HPP_ 

#include "coding_scheme.hpp"

class pbm: public coding_scheme {
    protected:
        unsigned int channels;    
        unsigned int * code_table;    
        void fv_encoder(std::istream &in, std::ostream &out);
        void fv_decoder(std::istream &in, std::ostream &out);

    public:
        pbm(unsigned int bitwidth);
        void encoder(std::istream &in, std::ostream &out);
        void decoder(std::istream &in, std::ostream &out);
};

#endif
