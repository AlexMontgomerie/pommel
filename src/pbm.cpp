#include "coding_schemes/pbm.hpp"

void pbm::fv_encoder(std::istream &in, std::ostream &out) { // TODO: add side channels
   
    // mask
    uint32_t mask = (1<<bitwidth)-1;

    // value buffer
    uint32_t val;

    // iterate over the rest of the stream
    while(in >> val) {
        out << code_table[(val & mask)] << std::endl;
    }
}

void pbm::encoder(std::istream &in, std::ostream &out) {
    // run encoder
    std::stringstream buf;
    fv_encoder(in, buf);
    decorrelator(buf, out);
}

pbm::pbm(unsigned int bitwidth) : coding_scheme(bitwidth){}
