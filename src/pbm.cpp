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

void pbm::encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out) {

    // send addresses in to addresses out
    addr_out << addr_in.rdbuf();

    // setup string streams
    std::stringstream fv_in;
    std::stringstream fv_intr;
    std::stringstream intr_decorr;

    // de-interleave stream in
    deinterleave(data_in, fv_in, platform.bitwidth, platform.packing_factor); 

    // frequent value encoder
    fv_encoder(fv_in, fv_intr);
       
    // interleave stream again
    interleave(fv_intr, intr_decorr, platform.bitwidth, platform.packing_factor); 

    // decorrrelation
    decorrelator(intr_decorr, data_out);
 
}

pbm::pbm(platform_config_t platform, std::map<uint32_t,uint32_t> code_table) : coding_scheme(platform), code_table(code_table) {}

}
