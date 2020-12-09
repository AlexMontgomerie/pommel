#include "common.hpp"

#include <boost/program_options.hpp>
using namespace boost::program_options;

#include "config.hpp"
#include "analysis.hpp"
#include "trace.hpp"
#include "convert.hpp"
#include "featuremap.hpp"
#include "encoder.hpp"

#include "coding_schemes/def.hpp"
#include "coding_schemes/abe.hpp"
#include "coding_schemes/pbm.hpp"
#include "coding_schemes/bi.hpp"
#include "coding_schemes/awr.hpp"
#include "coding_schemes/rle.hpp"


auto get_encoder_inst(std::string encoder_type, std::string config_path) {
    if( encoder_type == "bi" ) {    
        return silence::encoder<bi>(config_path);
    }
    else {
        fprintf(stderr,"ERROR (encoder) : %s not specified!\n", encoder_type.c_str());
    }
}

auto get_trace_inst(std::string dram_type ) {
    if( dram_type == "DDR3" ) { 
        return silence::trace<ramulator::DDR3>("outputs/test/ramulator_config.cfg","","",8);
    }
    else {
        fprintf(stderr,"ERROR (trace) : %s not specified!\n", dram_type.c_str());
    }
}

int main(int argc, char *argv[]) {

    // parameters
    int opt;
    
    // config file paths
    std::string arch_config_path;
    std::string memory_config_path;
    std::string encoder_config_path;
    std::string accelerator_config_path;

    // encoding scheme
    std::string coder_name;

    // file path names
    std::string featuremap_path;
    std::string scale_sim_trace_path;
    std::string trace_path;
    std::string data_path;
    std::string output_path;

    try {
 
        // add command line options
        options_description desc("Allowed Options");
        desc.add_options()
        ("help,h", "help message")
        ("memory", value(&memory_config_path)->required(), "file path for memory config (.xml)")
        ("encoder", value(&encoder_config_path)->required(), "file path for encoding scheme config (.xml)")
        ("accelerator", value(&accelerator_config_path)->required(), "accelerator config path (.xml)")
        ("featuremap", value(&featuremap_path)->required(), "featuremap data path (.h5)")
        ("output", value(&output_path)->required(), "output directory path")
        ;

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {  
            std::cout << desc << "\n";
            return 0;
        }

        notify(vm);

    }
    catch(std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 0;
    }

    // create configs
    silence::config config_inst;

    // load configs
    config_inst.load_memory_config(memory_config_path);
    config_inst.load_accelerator_config(accelerator_config_path);

    // iterate over partition for a given accelerator
    for(auto const& partition : config_inst.accelerator_config) { 

        // partition information
        int partition_index = partition.first;
        silence::accelerator_config_t partition_conf = partition.second;

        // run the input featuremap
        //  load the featuremap
        silence::featuremap featuremap_input(featuremap_path, partition_conf.input_featuremap);    
        featuremap_input.generate_stream("outputs/test/test.dat", partition_conf.transform);

        //  encode featuremap
        auto encoder_inst = get_encoder_inst("bi", "config/encoder/test_def.xml");    
        encoder_inst.encode_stream("outputs/test/test.dat", "outputs/test/test_encoded.dat");    

        // generate config for ramulator and trace
        config_inst.generate_ramulator_config("outputs/test/ramulator_config.cfg"); 
        
        // generate the trace
        auto fpgaconvnet_trace = get_trace_inst(config_inst.memory_config.dram_type);
        fpgaconvnet_trace.generate_trace("outputs/test/test.dat");

        // generate output configs
        config_inst.generate_cacti_config("outputs/test/cacti_config.cfg"); 

    }

    // end execution
    return 0;
}
