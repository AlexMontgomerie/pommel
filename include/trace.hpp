#ifndef TRACE_HPP_ 
#define TRACE_HPP_ 

#include "common.hpp"
#include <boost/format.hpp>

#include "Processor.h"
#include "Config.h"
#include "Controller.h"
#include "SpeedyController.h"
#include "Memory.h"
#include "DRAM.h"
#include "Statistics.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>
#include <functional>
#include <map>

/* Standards */
#include "Gem5Wrapper.h"
#include "DDR3.h"
#include "DDR4.h"
#include "DSARP.h"
#include "GDDR5.h"
#include "LPDDR3.h"
#include "LPDDR4.h"
#include "WideIO.h"
#include "WideIO2.h"
#include "HBM.h"
#include "SALP.h"
#include "ALDRAM.h"
#include "TLDRAM.h"
#include "STTMRAM.h"
#include "PCM.h"

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

    public:
        trace();

        void read_complete(unsigned id, uint64_t address, uint64_t clock_cycle);
        void write_complete(unsigned id, uint64_t address, uint64_t clock_cycle);

        void generate_trace(void);
        //std::fstream &tracefile;
};

}

#endif
