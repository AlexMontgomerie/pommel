#ifndef ANALYSIS_HPP_ 
#define ANALYSIS_HPP_ 

#include "common.hpp"
#include <bitset>
#include "stream_helper.hpp"

namespace pommel {

class analysis {
    
    protected:

        uint32_t addr_width;
        uint32_t data_width;

        std::vector<uint128_t> data_stream;
        std::vector<uint128_t> addr_stream;

        float get_total_transitions(std::vector<uint128_t> stream);

    public:

        float data_activity;
        float addr_activity;

        float data_mean;
        float data_variance;

        float get_addr_activity(void);
        float get_data_activity(void);
        float get_total_samples(void);
        float get_total_data_transitions(void);
        float get_total_addr_transitions(void);
        analysis(std::string trace_path, int data_width, int addr_width);
};

}

#endif
