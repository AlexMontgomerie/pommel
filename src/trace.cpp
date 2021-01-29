#include "trace.hpp"

namespace pommel {

template<>
trace<ramulator::DDR3>::trace(std::string ramulator_config_path, std::string output_path, int burst_size, int period, int bitwidth) : burst_size(burst_size), period(period), bitwidth(bitwidth) {
   
    // setup config
    setup_memory_config(ramulator_config_path, output_path);

    // setup memory specification
    spec = new ramulator::DDR3((*configs)["org"], (*configs)["speed"]);

    // setup memory
    setup_memory();

}

template<>
trace<ramulator::WideIO>::trace(std::string ramulator_config_path, std::string output_path, int burst_size, int period, int bitwidth) : burst_size(burst_size), period(period), bitwidth(bitwidth) {
   
    // setup config
    setup_memory_config(ramulator_config_path, output_path);

    // setup memory specification
    spec = new ramulator::WideIO((*configs)["org"], (*configs)["speed"]);

    // setup memory
    setup_memory();

}

template<>
trace<ramulator::LPDDR3>::trace(std::string ramulator_config_path, std::string output_path, int burst_size, int period, int bitwidth) : burst_size(burst_size), period(period), bitwidth(bitwidth) {
 
    // setup config
    setup_memory_config(ramulator_config_path, output_path);

    // setup memory specification
    spec = new ramulator::LPDDR3((*configs)["org"], (*configs)["speed"]);

    // setup memory
    setup_memory();

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
