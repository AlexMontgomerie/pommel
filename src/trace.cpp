#include "trace.hpp"

/**
 * Generate memory trace for ramulator simulator
 */

void streaming_trace(std::string tracefile_path, int fm_size, int burst_size, int bitwidth) {

    // create trace
    int in_addr  = 0;
    int out_addr = fm_size;
    std::ofstream tracefile(tracefile_path);
    for(int i=0; i<(int)(fm_size/burst_size); i++) {
        // input burst
        for( int j=0;j<burst_size;j++ ) {
            tracefile << boost::format("0x%X R") % in_addr << std::endl;
            in_addr += (int) (bitwidth/8);
        }
        // output burst
        for( int j=0;j<burst_size;j++ ) {
            tracefile << boost::format("0x%X W") % out_addr << std::endl;
            out_addr += (int) (bitwidth/8); 
        }
    }    
}


/*
void processing_engine_trace(std::string tracefile_path, fm_dim_t fm_in, int filters=128, int bitwidth=8) {

    // get the output dimensions
    int rows_out = ;
    int cols_out = ;

    // initialise base addresses
    int in_base_addr = 0;
    int out_base_addr = fm_in.rows*fm_in.cols*fm_in.channels;

    // tracefile setup
    std::ofstream tracefile(tracefile_path);
    
    // iterate over dimensions out
    for(int i=0;i<rows_out;i++) {
        for(int j=0;j<cols_out;j++) {
            // read input
            for(int k=0;k<fm_in.channels;k++) {
                for(int k1=0;k1<fm_in.kernel_size;k1++) {
                    for(int k2=0;k2<fm_in.kernel_size;k2++) {
                        int in_addr = in_base_addr + ( i * fm_in.cols * fm_in.channels +
                            j * fm_in.channels + 
                            k) + ( k1 * fm_in.kernel_size + k2 );
                    }
                }
            }
            // write output
            for(int k=0;k<filters;k++) {
                int out_addr = out_base_addr + ( i * cols_out * filters +
                        j * filters +
                        k) * ( (int) (bitwidth/8) );
                tracefile << std::format("0x%X W", out_addr) << std::endl;
            }
        }
    }

}
*/
