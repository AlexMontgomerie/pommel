#include "coding_schemes/pbm.hpp"

namespace pommel {

void pbm::fv_encoder(std::istream &in, std::ostream &out) { 
   
    // mask
    uint32_t mask = (1<<platform.bitwidth)-1;

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

pbm::pbm(platform_config_t platform) : coding_scheme(platform){}

}

