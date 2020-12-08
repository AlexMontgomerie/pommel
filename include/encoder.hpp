#ifndef ENCODER_HPP_
#define ENCODER_HPP_

#include "common.hpp"
#include "pugixml.hpp"
#include "coding_scheme.hpp"
#include "coding_schemes/def.hpp"
#include "coding_schemes/abe.hpp"
#include "coding_schemes/pbm.hpp"
#include "coding_schemes/bi.hpp"
#include "coding_schemes/awr.hpp"
#include "coding_schemes/rle.hpp"


namespace silence {

template<class T>
class encoder {
    protected:
        T * coder;
    public:
        encoder(std::string config_path);
        
        void encode_stream(std::string stream_in_path, std::string stream_out_path) {

            // open file streams
            std::ifstream in  (stream_in_path);
            if( !in.is_open() ) {
                fprintf(stderr,"cannot open input file: %s \n", stream_in_path);
            }
            std::ofstream out (stream_out_path);
            if( !out.is_open() ) {
                fprintf(stderr,"cannot open output file: %s \n", stream_out_path);
            } 

            // run encoder
            coder->encoder(in, out);
        
            // close files
            in.close();
            out.close();
        }

};

}

#endif
