#ifndef CONFIG_HPP_ 
#define CONFIG_HPP_ 

#include "common.hpp"
#include <ctemplate/template.h>
#include "pugixml.hpp"

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


namespace silence {

class config {
    protected:
        
        // all config parameters
        
        // memory parameters
        std::string dram_type;
        int data_width;
        int banks;
        int rank;
        int cols;
        int rows;
        int data_rate;
        int burst_length; 
        int clock;

    public:
        //config();

        void load_memory_config(std::string config_path);
        void load_platform_config(std::string config_path);

        void generate_ramulator_config(std::string config_path);
        void generate_cacti_config(std::string config_path);
        void generate_dram_power_config(std::string config_path);

};

}

#endif 
