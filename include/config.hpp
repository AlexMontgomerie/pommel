#ifndef CONFIG_HPP_ 
#define CONFIG_HPP_ 

#include "common.hpp"
#include <ctemplate/template.h>
#include "pugixml.hpp"
#include "featuremap.hpp"
#include "csv.h"

#include "ramulator/src/Config.h"
#include "ramulator/src/Controller.h"
#include "ramulator/src/SpeedyController.h"
#include "ramulator/src/Memory.h"
#include "ramulator/src/DRAM.h"
#include "ramulator/src/Statistics.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>
#include <functional>
#include <map>

/* Standards */
#include "ramulator/src/DDR3.h"
#include "ramulator/src/DDR4.h"
#include "ramulator/src/DSARP.h"
#include "ramulator/src/GDDR5.h"
#include "ramulator/src/LPDDR3.h"
#include "ramulator/src/LPDDR4.h"
#include "ramulator/src/WideIO.h"
#include "ramulator/src/WideIO2.h"
#include "ramulator/src/HBM.h"
#include "ramulator/src/SALP.h"
#include "ramulator/src/ALDRAM.h"
#include "ramulator/src/TLDRAM.h"
#include "ramulator/src/STTMRAM.h"
#include "ramulator/src/PCM.h"


namespace pommel {

typedef struct {
    std::string dram_type;
    int num_dq;
    int num_chips;
    int data_width;
    int addr_width;
    int banks;
    int rank;
    int cols;
    int rows;
    int data_rate;
    int burst_length; 
    int clock;
    int capacity;
    float bandwidth;
} memory_config_t;

typedef struct {
    int kernel_size;
    int stride;
    float bandwidth_in;
    float bandwidth_out;
    std::string input_featuremap;
    std::string output_featuremap;
} network_config_t;

typedef struct {
    int bitwidth;
    int packing_factor;
    int burst_size;
    int clk_freq;
    int array_height;
    int array_width;
    int ifmap_sram_size;
    int filter_sram_size;
    int ofmap_sram_size;
    int ifmap_offset = 0;
    int filter_offset = 10000000;
    int ofmap_offset = 20000000;
    std::string dataflow;
    std::string transform;
} platform_config_t;

class config {
    public:

        // memory config type
        memory_config_t memory;

        // memory config type
        platform_config_t platform;

        // accelerator config type
        std::map<int, network_config_t> network;

        // load config functions
        void load_memory_config(std::string config_path);
        void load_network_config(std::string config_path);
        void load_platform_config(std::string config_path);

        // generate config functions
        void generate_ramulator_config(std::string config_path);
        //void generate_cacti_config(std::string config_path);
        void generate_cacti_config(std::string direction, std::string config_path, float data_activity, float address_activity, float duty_cycle);
        void generate_dram_power_config(std::string config_path);
        void generate_scale_sim_config(std::string config_path, std::string output_path); 
        void generate_scale_sim_topology(std::string featuremap_path, std::string topology_path);


        void add_scale_sim_bandwidth(std::string report_path);
};

}

#endif 
