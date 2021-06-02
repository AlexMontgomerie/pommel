#include "coding_schemes/general.hpp"

namespace pommel {

void general::encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out) {

    // initialise value
    uint32_t prev[platform.bitwidth*platform.packing_factor];

    while( data_in.rdbuf()->in_avail() ) {
        // create buffer
        std::stringstream data_buffer;
        std::stringstream addr_buffer;
        std::string data_line;
        std::string addr_line;

        for(int i=0; i<batch_size; i++) {
            // read in streams
            std::getline(data_in, data_line);
            std::getline(addr_in, addr_line);
            // check if empty
            if( data_line.empty() || addr_line.empty() )
                break;
            // write to buffer
            data_buffer << data_line << std::endl;
            addr_buffer << addr_line << std::endl;
        }

        // encode buffer
        std::stringstream encoded_data_buffer;

        // setup string streams
        std::stringstream bitstream;
        std::stringstream bitstream_compressed;

        // de-interleave stream in
        deinterleave(data_buffer, bitstream, 1, platform.bitwidth*platform.packing_factor);

        // only select samples within the compression ratio
        int sample_limit = (int) ( ( (float) batch_size*platform.bitwidth*platform.packing_factor ) / compression_ratio );
        int index = 0;
        while( std::getline(bitstream, data_line) ) {
            if ( index < sample_limit ) {
                // get a random value between 0 and 1
                float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                // flip bits based on activity
                if( r < activity ) {
                    prev[index%(platform.bitwidth*platform.packing_factor)] = ~prev[index%(platform.bitwidth*platform.packing_factor)];
                } else {
                    prev[index%(platform.bitwidth*platform.packing_factor)] = prev[index%(platform.bitwidth*platform.packing_factor)];
                }
                /* bitstream_compressed << data_line << std::endl; */
                bitstream_compressed << prev[index%(platform.bitwidth*platform.packing_factor)] << std::endl;
            }
            index++;
        }
        // interleave stream again
        interleave(bitstream_compressed, encoded_data_buffer, 1, platform.bitwidth*platform.packing_factor);

        // send to output
        std::string addr_line_prev;
        while( std::getline(encoded_data_buffer, data_line) ) {
            // read address buffer also
            std::getline(addr_buffer, addr_line);
            if( addr_line.empty() ) {
                addr_line = addr_line_prev;
            }
            // write to output streams
            addr_line_prev = addr_line;
            data_out << data_line << std::endl;
            addr_out << addr_line << std::endl;
        }
    }
}

general::general(platform_config_t platform, float compression_ratio, float activity, unsigned int batch_size) : coding_scheme(platform), compression_ratio(compression_ratio), activity(activity), batch_size(batch_size){}

}
