#include "trace.hpp"

namespace silence {
/*
std::string get_line(std::fstream& file, unsigned int line) {
    file.seekg(std::ios::beg);
    for(int i=0; i < line - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    std::string out;
    file >> out;
    return out;

}

void add_data(std::string trace_in_file_path, std::string trace_out_file_path, std::string datafile_path, int addr_offset, bool use_addr) {

    // open traceinfile
    std::ifstream trace_in_file(trace_in_file_path);
    if(!trace_in_file.is_open()) throw std::runtime_error("Could not open traceinfile!");

    // open traceoutfile
    std::ofstream trace_out_file(trace_out_file_path);
    if(!trace_out_file.is_open()) throw std::runtime_error("Could not open traceoutfile!");

    // open datafile
    std::fstream datafile(datafile_path);
    if(!datafile.is_open()) throw std::runtime_error("Could not open datafile!");

    // iterate over lines in the file in
    std::string line;
    std::string val;
    int index = 0;
    while( std::getline(trace_in_file,line) ) {
        
        index++;
        
        // convert line to string stream
        std::stringstream row(line);
        
        // get the addr 
        std::getline(row, val, ' ');
        int addr = std::stoul(val, nullptr, 16);

        // get the direction
        std::getline(row, val, ' ');
        char direction = (char) val[0];

        // get data
        int data;
        if ( use_addr ) {
            data = stoi( get_line(datafile, addr-addr_offset + 1 ) );
        } else {
            data = stoi( get_line(datafile, index ) );
        }

        // save to outfile
        char buffer[50];
        sprintf(buffer, "0x%X %c %d\n", addr, direction, data);
        trace_out_file << buffer; 

    }

    // close files
    trace_in_file.close();
    trace_out_file.close();
    datafile.close();
}

void get_data(std::string tracefile_path, std::string datafile_path) {


    // open tracefile
    std::ifstream tracefile(tracefile_path);
    if(!tracefile.is_open()) throw std::runtime_error("Could not open trace file!");

    // open datafile
    std::ofstream datafile(datafile_path);
    if(!datafile.is_open()) throw std::runtime_error("Could not open datafile!");

    // iterate over lines in the file in
    std::string line;
    std::string val;
    while( std::getline(tracefile,line) ) {
        
        // convert line to string stream
        std::stringstream row(line);
        
        // get the addr 
        std::getline(row, val, ' ');
        int addr = std::stoul(val, nullptr, 16);

        // get the direction
        std::getline(row, val, ' ');
        char direction = (char) val[0];

        // get data
        try {
            std::getline(row, val, ' ');
            int data = stoi(val);
    
            // save to data file
            char buffer[50];
            sprintf(buffer, "%d\n", data);
            datafile << buffer; 

        } 
        catch( std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
        }

    }

    // close files
    tracefile.close();
    datafile.close();

}
*/

trace::trace() {
    bitwidth = 8;
    burst_size = 256;
    period = 512;
    direction = true;
    size = 2048;
}

/*
void trace::read_complete(unsigned id, uint64_t address, uint64_t clock_cycle)
{
        printf("[Callback] read complete: %d 0x%lx cycle=%lu\n", id, address, clock_cycle);
}

void trace::write_complete(unsigned id, uint64_t address, uint64_t clock_cycle)
{
        printf("[Callback] write complete: %d 0x%lx cycle=%lu\n", id, address, clock_cycle);
}

void power_callback(double a, double b, double c, double d) {}
*/

template<typename T>
void trace::run_trace(const ramulator::Config& configs, T* spec) {

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


    for(int i=0;i< floor(size/burst_size) ;i++) {

        for(int j=0;j<burst_size;j++) {
            req.addr = i*10+j;
            req.type = type;
            memory.send(req);
            //memory.tick();
            //clks ++;
            //Stats::curTick++; // memory clock, global, for Statistics
        }
        for(int j=0;j<period-burst_size;j++) {
            memory.tick();
            clks ++;
            Stats::curTick++; // memory clock, global, for Statistics
        } 

    }

    // This a workaround for statistics set only initially lost in the end
    memory.finish();
    Stats::statlist.printall();

}

void trace::generate_trace(void) {

    ramulator::Config configs("ramulator/configs/DDR3-config.cfg");
    
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
    
    //mem_type* spec = new mem_type(configs["org"], configs["speed"]);
 
    if (standard == "ramulator::DDR3") {
      ramulator::DDR3* ddr3 = new ramulator::DDR3(configs["org"], configs["speed"]);
      run_trace(configs, ddr3);
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
   

}
