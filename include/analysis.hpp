#ifndef ANALYSIS_HPP_ 
#define ANALYSIS_HPP_ 

#include "common.hpp"
#include <bitset>
#include "stream_helper.hpp"

namespace silence {

class analysis {
    
    protected:

        uint32_t addr_width;
        uint32_t data_width;

        std::vector<uint32_t> data_stream;
        std::vector<uint32_t> addr_stream;

        float get_total_transitions(std::vector<uint32_t> stream);

    public:

        float data_activity;
        float addr_activity;

        float data_mean;
        float data_variance;

        float get_addr_activity(void);
        float get_data_activity(void);
        analysis(std::string trace_path, int data_width, int addr_width);
};

}

/*
std::vector<float> bitwise_mean(std::string infile_path, unsigned int bitwidth);
std::vector<uint32_t> bitwise_variance(std::istream &in, unsigned int bitwidth);
std::vector<uint32_t> bitwise_transitions(std::istream &in, unsigned int bitwidth);
std::vector<uint32_t> bitwise_switching_activity_mean(std::istream &in, unsigned int bitwidth);
std::vector<uint32_t> bitwise_switching_activity_variance(std::istream &in, unsigned int bitwidth);

float mean(std::string infile_path, unsigned int bitwidth);
uint32_t variance(std::istream &in, unsigned int bitwidth);
uint32_t transitions(std::istream &in, unsigned int bitwidth);
uint32_t switching_activity_mean(std::istream &in, unsigned int bitwidth);
uint32_t switching_activity_variance(std::istream &in, unsigned int bitwidth);
*/

#endif
