#ifndef TRACE_HPP_ 
#define TRACE_HPP_ 

#include "common.hpp"
#include "stream_helper.hpp"
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

namespace pommel {

template<class T>
class trace {
    protected:
        
        // accelerator parameters
        unsigned int bitwidth;
        unsigned int burst_size;
        unsigned int period;

        // ramulator config instance
        ramulator::Config * configs;

        // memory instance
        ramulator::Memory<T, ramulator::Controller> * memory;

        // memory spec instance
        T * spec;

        // function to setup memory
        void setup_memory(void) {
            // initiate controller and memory
            int C = configs->get_channels(), R = configs->get_ranks();
            // Check and Set channel, rank number
            spec->set_channel_number(C);
            spec->set_rank_number(R);
            // iterate over the controllers for each channel
            std::vector<ramulator::Controller<T>*> ctrls;
            for (int c = 0 ; c < C ; c++) {
                ramulator::DRAM<T>* channel = new ramulator::DRAM<T>(spec, T::Level::Channel);
                channel->id = c;
                channel->regStats("");
                ramulator::Controller<T>* ctrl = new ramulator::Controller<T>(*configs, channel);
                ctrls.push_back(ctrl);
            }
            // initialise memory type 
            memory = new ramulator::Memory<T, ramulator::Controller>(*configs, ctrls);
        }

        // function to setup config
        void setup_memory_config(std::string ramulator_config_path, std::string output_path) {
            
            // create new ramulator config instance
            configs = new ramulator::Config(ramulator_config_path);

            // add cmd trace prefix
            configs->add("cmd_trace_prefix",output_path.c_str());

            const std::string& standard = (*configs)["standard"];
            assert(standard != "" || "DRAM standard should be specified.");

            configs->add("trace_type", "DRAM");

            int trace_start = 3;
            //string stats_out;
            //Stats::statlist.output(standard+".stats");
            //stats_out = standard + string(".stats");

            // A separate file defines mapping for easy config.
            configs->add("mapping", "defaultmapping");

            // set core number
            configs->set_core_num(1);
         }

    public:
        
        //trace(std::string ramulator_config_path, std::string accelerator_config_path, std::string output_path, int bitwidth);
        trace(std::string ramulator_config_path, std::string output_path, int burst_size, int period, int bitwidth);
        
        void generate_trace(std::string stream_path) {
            
            // setup requests
            map<int, int> latencies;
            auto read_complete = [&latencies](ramulator::Request& r){latencies[r.depart - r.arrive]++;};
            ramulator::Request req(0, ramulator::Request::Type::WRITE, read_complete);

            // load stream in
            std::ifstream in  (stream_path);
            if( !in.is_open() ) {
                perror("file error");
                fprintf(stderr,"cannot open input file: %s \n", stream_path.c_str());
            }
            
            // iterate over stream
            while( in.rdbuf()->in_avail() ) {
                // iterate over batch
                for(int i=0;i<burst_size;i++) {
                    // break if buffer empty
                    if( !in.rdbuf()->in_avail() ) {
                        break;
                    }

                    // read from buffer
                    std::string line;
                    std::getline(in, line);

                    // convert line to string stream
                    std::stringstream row(line);
                    std::string val;

                    // get address
                    std::getline(row,val,' ');
                    int addr = stoi(val);

                    // get direction
                    std::getline(row,val,' ');
                    ramulator::Request::Type RW = (val == "R") ? 
                        ramulator::Request::Type::READ : ramulator::Request::Type::WRITE;

                    // send to memory        }
                    req.addr = addr;
                    req.type = RW;
                    memory->send(req);
                    memory->tick();
                }

                // iterate over rest of burst
                for(int i=0;i<period-burst_size;i++) {
                    memory->tick();
                } 
            }

            // finish memory transactions
            memory->finish();

            // close file in
            in.close();

            delete configs;
            delete memory;
        }

        void generate_combined_trace(std::string stream_in_path, std::string stream_out_path) {
            
            // setup requests
            map<int, int> latencies;
            auto read_complete = [&latencies](ramulator::Request& r){latencies[r.depart - r.arrive]++;};
            ramulator::Request req(0, ramulator::Request::Type::WRITE, read_complete);

            // load stream in
            std::ifstream in (stream_in_path);
            if( !in.is_open() ) {
                perror("file error");
                fprintf(stderr,"cannot open input file: %s \n", stream_in_path.c_str());
            }

            // load stream out
            std::ifstream out (stream_out_path);
            if( !out.is_open() ) {
                perror("file error");
                fprintf(stderr,"cannot open output file: %s \n", stream_out_path.c_str());
            }
           
            // get all streams
            std::stringstream addr_in_stream;
            get_stream_field(in, addr_in_stream, ADDR);
                
            std::stringstream addr_out_stream;
            get_stream_field(out, addr_out_stream, ADDR);

            // iterate over stream
            uint32_t addr_val;
            while( addr_in_stream.rdbuf()->in_avail() || addr_out_stream.rdbuf()->in_avail() ) {

                // size of buffers
                int buffer_size_in = 0;
                int buffer_size_out = 0;

                // iterate over batch
                for(int i=0;i<burst_size;i++) {

                    // log buffer size
                    buffer_size_in++;

                    // break if buffer empty
                    if( !addr_in_stream.rdbuf()->in_avail() ) {
                        break;
                    }
                    
                    // read in address val
                    addr_in_stream >> addr_val;

                    // send to memory        }
                    req.addr = addr_val;
                    req.type = ramulator::Request::Type::READ;
                    memory->send(req);
                    memory->tick();
                }

                // iterate over batch
                for(int i=0;i<burst_size;i++) {

                    // log buffer size
                    buffer_size_out++;

                    // break if buffer empty
                    if( !addr_out_stream.rdbuf()->in_avail() ) {
                        break;
                    }
                    
                    // read in address val
                    addr_out_stream >> addr_val;

                    // send to memory        }
                    req.addr = addr_val;
                    req.type = ramulator::Request::Type::WRITE;
                    memory->send(req);
                    memory->tick();
                }

                // iterate over rest of burst
                for(int i=0;i<period-buffer_size_in-buffer_size_out;i++) {
                    memory->tick();
                } 
            }

            // finish memory transactions
            memory->finish();

            // close file in and out
            in.close();
            out.close();

            delete configs;
            delete memory;
        }

};

}

#endif
