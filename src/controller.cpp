#include "controller.hpp"

namespace pommel {

uint32_t standard_controller(config &config_inst, network_config_t &partition_conf, float compression_ratio_in, float compression_ratio_out) {

    // find the actual bandwidth in and out
    float bandwidth_in  = partition_conf.bandwidth_in  / ( compression_ratio_in );
    float bandwidth_out = partition_conf.bandwidth_out / ( compression_ratio_out );

    // get period (clk cycles) for input and output featuremaps
    int period_in   = (int) ( config_inst.platform.burst_size*config_inst.memory.bandwidth / bandwidth_in );
    int period_out  = (int) ( config_inst.platform.burst_size*config_inst.memory.bandwidth / bandwidth_out );

    // calculate burst transfer period
    uint32_t period = std::max(period_in, period_out);
    if ( period <= 2*config_inst.platform.burst_size ) {
        printf("WARNING: required bandwidth larger than memory bandwidth\n");
        period = 2*config_inst.platform.burst_size+1;
    }

    // return the period
    return period;
}

uint32_t variable_frequency_controller(config &config_inst, network_config_t &partition_conf, float compression_ratio_in, float compression_ratio_out) {

    // reset memory bandwidth
    config_inst.memory.clock = config_inst.memory.base_clock;
    config_inst.update_memory_bandwidth();

    // find the actual bandwidth in and out
    float bandwidth_in  = partition_conf.bandwidth_in  / ( compression_ratio_in );
    float bandwidth_out = partition_conf.bandwidth_out / ( compression_ratio_out );

    // get the maximum bandwidth of the two
    float bandwidth = bandwidth_in + bandwidth_out;

    // get the amount to scale frequency
    float scale = bandwidth / config_inst.memory.bandwidth;

    // scale the frequency
    config_inst.memory.clock = scale*config_inst.memory.base_clock;

    // update the memory bandwidth
    config_inst.update_memory_bandwidth();

    // return the max bandwidth for this frequency
    return 2*config_inst.platform.burst_size+1;

}

}
