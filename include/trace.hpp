#ifndef TRACE_HPP_ 
#define TRACE_HPP_ 

#include "common.hpp"
#include <boost/format.hpp>

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

class trace {
    protected:
        
        // parameters for the trace
        unsigned int bitwidth;
        unsigned int burst_size;
        unsigned int period;
        unsigned int size;
        bool direction;

        // memory system from DRAMSim2
        //DRAMSim::MemorySystem &mem;
        template<typename T>
        void run_trace(const ramulator::Config& configs, T* spec);

    public:
        trace();

        void read_complete(unsigned id, uint64_t address, uint64_t clock_cycle);
        void write_complete(unsigned id, uint64_t address, uint64_t clock_cycle);

        void generate_trace(void);
        //std::fstream &tracefile;
};

}

#endif
