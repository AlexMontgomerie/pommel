#include "coding_schemes/pbm.hpp"

namespace pommel {

void pbm::fv_encoder(std::istream &in, std::ostream &out) { 
   
    // value buffer
    uint32_t val;

    // iterate over the rest of the stream
    while(in >> val) {
        out << code_table[val] << std::endl;
    }
}

void pbm::encoder(std::istream &in, std::ostream &out) {

    // setup string streams
    std::stringstream fv_in;
    std::stringstream fv_intr;
    std::stringstream intr_decorr;

    // de-interleave stream in
    deinterleave(in, fv_in, platform.bitwidth, platform.packing_factor); 

    // frequent value encoder
    fv_encoder(fv_in, fv_intr);
       
    // interleave stream again
    interleave(fv_intr, intr_decorr, platform.bitwidth, platform.packing_factor); 

    // decorrrelation
    decorrelator(intr_decorr, out);
 
}

pbm::pbm(platform_config_t platform, std::map<uint32_t,uint32_t> code_table) : coding_scheme(platform), code_table(code_table) {}

}
