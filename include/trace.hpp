#ifndef TRACE_HPP_ 
#define TRACE_HPP_ 

#include "common.hpp"
#include <DRAMSim.h>
#include <boost/format.hpp>

typedef struct {
    int batch_size;
    int rows, cols, channels;
    int kernel_size;
    int stride;
} fm_dim_t;


void add_data(std::string trace_in_file_path, std::string trace_out_file_path, std::string datafile_path, int addr_offset=0x0, bool use_addr=true); 
void get_data(std::string tracefile_path, std::string datafile_path);

void streaming_trace(std::string tracefile_path, int fm_size=1000, int burst_size=64, int bitwidth=8);

#endif
