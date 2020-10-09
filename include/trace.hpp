#ifndef TRACE_HPP_ 
#define TRACE_HPP_ 

#include "common.hpp"
#include <boost/format.hpp>

typedef struct {
    int batch_size;
    int rows, cols, channels;
    int kernel_size;
    int stride;
} fm_dim_t;


void streaming_trace(std::string tracefile_path, int fm_size=1000, int burst_size=64, int bitwidth=8);

#endif
