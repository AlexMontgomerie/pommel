#include "trace.hpp"

namespace silence {

template<typename T>
void trace::run_trace(std::string stream_path, const ramulator::Config& configs, T* spec) {

    // initiate controller and memory
    int C = configs.get_channels(), R = configs.get_ranks();
    // Check and Set channel, rank number
    spec->set_channel_number(C);
    spec->set_rank_number(R);
    std::vector<ramulator::Controller<T>*> ctrls;
    for (int c = 0 ; c < C ; c++) {
        ramulator::DRAM<T>* channel = new ramulator::DRAM<T>(spec, T::Level::Channel);
        channel->id = c;
        channel->regStats("");
        ramulator::Controller<T>* ctrl = new ramulator::Controller<T>(configs, channel);
        ctrls.push_back(ctrl);
    }
    ramulator::Memory<T, ramulator::Controller> memory(configs, ctrls);

    bool stall = false, end = false;
    int reads = 0, writes = 0, clks = 0;
    long addr = 0;

    // get the read/write type
    ramulator::Request::Type type;
    if ( direction ) {
        type = ramulator::Request::Type::WRITE;
    } else {
        type = ramulator::Request::Type::READ;
    }

    map<int, int> latencies;
    auto read_complete = [&latencies](ramulator::Request& r){latencies[r.depart - r.arrive]++;};

    ramulator::Request req(addr, type, read_complete);

    // load stream in
    std::ifstream in  (stream_path);
    if( !in.is_open() ) {
        fprintf(stderr,"cannot open input file: %s \n", stream_path);
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

            // send to memory
            req.addr = addr;
            req.type = RW;
            memory.send(req);
            memory.tick();
        }

        // iterate over rest of burst
        for(int i=0;i<period-burst_size;i++) {
            memory.tick();
        } 

    }

    // This a workaround for statistics set only initially lost in the end
    memory.finish();
    Stats::statlist.printall();

}

void trace::generate_trace(std::string stream_path, std::string ramulator_config_path) {

    ramulator::Config configs(ramulator_config_path);
    
    const std::string& standard = configs["standard"];
    assert(standard != "" || "DRAM standard should be specified.");

    configs.add("trace_type", "DRAM");

    int trace_start = 3;
    string stats_out;
    Stats::statlist.output(standard+".stats");
    stats_out = standard + string(".stats");

    // A separate file defines mapping for easy config.
    configs.add("mapping", "defaultmapping");

    configs.set_core_num(1);
    
    if (standard == "DDR3") {
      ramulator::DDR3* ddr3 = new ramulator::DDR3(configs["org"], configs["speed"]);
      run_trace(stream_path, configs, ddr3);
    } 
    /*
    else if (standard == "ramulator::DDR4") {
      ramulator::DDR4* ddr4 = new ramulator::DDR4(configs["org"], configs["speed"]);
      run_trace(configs, ddr4);
    } else if (standard == "ramulator::SALP-MASA") {
      ramulator::SALP* salp8 = new ramulator::SALP(configs["org"], configs["speed"], "ramulator::SALP-MASA", configs.get_subarrays());
      run_trace(configs, salp8);
    } else if (standard == "ramulator::LPramulator::DDR3") {
      ramulator::LPDDR3* lpddr3 = new ramulator::LPDDR3(configs["org"], configs["speed"]);
      run_trace(configs, lpddr3);
    } else if (standard == "ramulator::LPramulator::DDR4") {
      // total cap: 2GB, 1/2 of others
      ramulator::LPDDR4* lpddr4 = new ramulator::LPDDR4(configs["org"], configs["speed"]);
      run_trace(configs, lpddr4);
    } else if (standard == "ramulator::GDDR5") {
      ramulator::GDDR5* gddr5 = new ramulator::GDDR5(configs["org"], configs["speed"]);
      run_trace(configs, gddr5);
    } else if (standard == "ramulator::HBM") {
      ramulator::HBM* hbm = new ramulator::HBM(configs["org"], configs["speed"]);
      run_trace(configs, hbm);
    } else if (standard == "ramulator::WideIO") {
      // total cap: 1GB, 1/4 of others
      ramulator::WideIO* wio = new ramulator::WideIO(configs["org"], configs["speed"]);
      run_trace(configs, wio);
    } else if (standard == "ramulator::ramulator::WideIO2") {
      // total cap: 2GB, 1/2 of others
      ramulator::WideIO2* wio2 = new ramulator::WideIO2(configs["org"], configs["speed"], configs.get_channels());
      wio2->channel_width *= 2;
      run_trace(configs, wio2);
    } else if (standard == "STTMRAM") {
        ramulator::STTMRAM* sttmram = new ramulator::STTMRAM(configs["org"], configs["speed"]);
        run_trace(configs, sttmram);
    } else if (standard == "ramulator::PCM") {
        ramulator::PCM* pcm = new ramulator::PCM(configs["org"], configs["speed"]);
        run_trace(configs, pcm);
    } else if (standard == "ramulator::DSARP") {
      ramulator::DSARP* dsddr3_dsarp = new ramulator::DSARP(configs["org"], configs["speed"], ramulator::DSARP::Type::DSARP, configs.get_subarrays());
      run_trace(configs, dsddr3_dsarp);
    } else if (standard == "ramulator::ALDRAM") {
      ramulator::ALDRAM* aldram = new ramulator::ALDRAM(configs["org"], configs["speed"]);
      run_trace(configs, aldram);
    } else if (standard == "ramulator::TLDRAM") {
      ramulator::TLDRAM* tldram = new ramulator::TLDRAM(configs["org"], configs["speed"], configs.get_subarrays());
      run_trace(configs, tldram);
    }
    */

}

trace::trace() {
    bitwidth = 8;
    burst_size = 256;
    period = 512;
    direction = true;
    size = 2048;
}

}
