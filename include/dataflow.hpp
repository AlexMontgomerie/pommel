#ifndef DATAFLOW_HPP_ 
#define DATAFLOW_HPP_ 

#include "common.hpp"

class fpgaconvnet_dataflow {
    protected:
        unsigned int streams, rows, cols, channels;
    public:
        fpgaconvnet_dataflow(unsigned int streams, unsigned int rows,
            unsigned int cols, unsigned int channels);
        void gen_memory_trace(std::string infile_path, std::string outfile_path);
};

#endif
