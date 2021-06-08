#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include "common.hpp"
#include "config.hpp"

namespace pommel {

uint32_t standard_controller(config &config_inst, network_config_t &partition_conf, float compression_ratio_in, float compression_ratio_out);
uint32_t variable_frequency_controller(config &config_inst, network_config_t &partition_conf, float compression_ratio_in, float compression_ratio_out);

}

#endif
