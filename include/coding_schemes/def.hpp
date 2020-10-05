#ifndef CODING_SCHEMES_DEF_HPP_ 
#define CODING_SCHEMES_DEF_HPP_ 

#include "coding_scheme.hpp"

class def: public coding_scheme {
    protected:
        unsigned int channels;    
        int int_to_smint(int in);
        int smint_to_int(int in);
        void diff_encoder(std::istream &in, std::ostream &out);
        void diff_decoder(std::istream &in, std::ostream &out);

    public:
        def(unsigned int bitwidth, unsigned int channels);
        void encoder(std::istream &in, std::ostream &out);
        void decoder(std::istream &in, std::ostream &out);
};

#endif
