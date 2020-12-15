#ifndef ENCODER_HPP_
#define ENCODER_HPP_

#include "common.hpp"
#include <boost/format.hpp>
#include "pugixml.hpp"
#include "stream_helper.hpp"
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
        encoder(std::string config_path, std::string featuremap, int bitwidth);

        void encode_stream(std::string stream_in_path, std::string stream_out_path) {

            // open stream in
            std::ifstream in  (stream_in_path);
            if( !in.is_open() ) {
                fprintf(stderr,"cannot open input file: %s \n", stream_in_path.c_str());
            }
    
            // get the data field stream
            std::stringstream data_stream;
            get_stream_field(in, data_stream, DATA);
            
            // reset to beginning of input stream
            in.clear();
            in.seekg(0, std::ios::beg);

            // run encoder
            std::stringstream encoded_data_stream;
            coder->encoder(data_stream, encoded_data_stream);
 
            // open stream out
            std::ofstream out (stream_out_path);
            if( !out.is_open() ) {
                fprintf(stderr,"cannot open output file: %s \n", stream_out_path.c_str());
            } 

            // add data to stream out
            add_stream_field(in, encoded_data_stream, out, DATA);
       
            // close files
            in.close();
            out.close();
        }

};

}

#endif
