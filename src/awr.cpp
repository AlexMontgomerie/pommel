#include "coding_schemes/awr.hpp"

namespace pommel {

void awr::encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out) {
 
    // send addresses in to addresses out
    addr_out << addr_in.rdbuf();
  
    // mask
    uint32_t mask = (1<<platform.bitwidth)-1;

    // value buffer
    uint32_t val;
    uint32_t prev_val = 0;

    // iterate over the rest of the stream
    while( !data_in.eof() ) {
       
        // get a window from stream
        std::vector<uint32_t> window;
        for(int i=0;i<N;i++) {
            if( data_in.eof() ) {
                break;
            }
            data_in >> val;
            window.push_back(val);
        }

        // make a copy of the window
        std::vector<uint32_t> window_copy(window);

        // iterate over window
        for(int i=0; i<window_copy.size(); i++) {
            
            // get minimum distance
            uint32_t min_distance = 0xFFFFFFFF;
            int min_distance_idx = 0;
            for(int j=0; j<window.size(); j++) {
                uint32_t distance = (__builtin_popcount(window[j]^prev_val));
                if( distance < min_distance ) {
                    min_distance = distance;
                    min_distance_idx = j;
                } 
            }

            // get the value to transmit
            val = window[min_distance_idx];
            window.erase(window.begin()+min_distance_idx);
            prev_val = val;

            // get side channel index
            int sc_idx = 0;
            for(int j=0; j<window_copy.size(); j++) {
                if( val == window_copy[j] ) {
                    sc_idx = j;
                }
            }

            // transmit value along with index
            data_out << ( val | (sc_idx << platform.bitwidth) ) << std::endl;

        }            
    }
}

awr::awr(platform_config_t platform, unsigned int N) : coding_scheme(platform), N(N){}

}
