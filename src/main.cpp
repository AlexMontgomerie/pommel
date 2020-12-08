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


/*
using coder_t = std::variant<def,abe,pbm,bi,awr,rle>;

coder_t get_coder(std::string coder_name) {

    if (coder_name == "def") {
        return def(8,1);
    } else if (coder_name == "abe") {
        return abe(8,32);
    } else if (coder_name == "bi") {
        return bi(8);
    } else if (coder_name == "awr") {
        return awr(8,4);
    } else if (coder_name == "rle") {
        return rle(8,0);
    }
}
*/

int main(int argc, char *argv[]) {

    // parameters
    int opt;
    
    // config file paths
    std::string arch_config_path;
    std::string memory_config_path;
    std::string model_config_path;

    // encoding scheme
    std::string coder_name;

    // file path names
    std::string scale_sim_trace_path;
    std::string trace_path;
    std::string data_path;
    std::string output_path;

    try {
 
    // add command line options
    options_description desc("Allowed Options");
    desc.add_options()
    ("help,h", "help message")
    ("arch", value(&arch_config_path), "file path for architecture config")
    ("memory", value(&memory_config_path), "file path for memory config")
    ("model", value(&model_config_path), "file path for model config")
    ("encoder,e", value(&coder_name)->required(), "encoding scheme name")
    ("scale-sim-trace", value(&scale_sim_trace_path), "file path to scale sim trace (optional)")
    ("trace", value(&trace_path), "file path to memory trace (optional)")
    ("data", value(&data_path), "file path to data (optional)")
    ("output,o", value(&output_path), "file path to output (optional)")
    ;

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {  
        std::cout << desc << "\n";
        return 0;
    }

    notify(vm);

    }
    catch(required_option& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 0;
    }
    catch(std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 0;
    }

    
    // create configs
    silence::config fpgaconvnet_config;

    // load configs
    fpgaconvnet_config.load_memory_config("config/memory/MICRON_1Gb_DDR3-1600_8bit_G.xml");

    // load the featuremap
    silence::featuremap fpgaconvnet_featuremap("data/test.h5", "classifier.1");    
    fpgaconvnet_featuremap.generate_stream("outputs/test/test.dat", "channel-first");

    // encode featuremap
    if( coder_name == "bi" ) {
        silence::encoder<bi> fpgaconvnet_encoder("config/encoder/test_def.xml");
        fpgaconvnet_encoder.encode_stream("outputs/test/test.dat", "outputs/test/test_encoded.dat");    
    } else if( coder_name == "def" ) {
        silence::encoder<bi> fpgaconvnet_encoder("config/encoder/test_def.xml");
        fpgaconvnet_encoder.encode_stream("outputs/test/test.dat", "outputs/test/test_encoded.dat");    
    } else {
        fprintf(stderr,"ERROR (encoder) : %s not specified!\n", coder_name.c_str());
        return 1;
    }

    // generate config for ramulator and trace
    fpgaconvnet_config.generate_ramulator_config("outputs/test/ramulator_config.cfg"); 
    
    // generate the trace
    silence::trace fpgaconvnet_trace;
    fpgaconvnet_trace.generate_trace("outputs/test/test.dat","outputs/test/ramulator_config.cfg");

    // generate output configs
    fpgaconvnet_config.generate_cacti_config("outputs/test/cacti_config.cfg"); 

    /*
    if(scale_sim_trace_path != "") {
        //scale_sim_to_memory_trace("dram_filter.csv", "tmp.trace");
    }
    scale_sim_to_memory_trace("dram_filter.csv", "tmp.trace");

    add_data("tmp.trace", "tmp_out.trace", "data/test.dat", 0x989680, true); // TODO: get offset from arch config
    
    get_data("tmp_out.trace", "tmp.dat");
 
   
    // load the input file stream
    std::ifstream in  ("tmp.dat");
    std::ofstream out ("tmp_out.dat");

    // run the encoder
    std::visit([&in=in,&out=out](auto&& arg){ arg.encoder(in,out); }, coder);

    // close files
    in.close();
    out.close();

    add_data("tmp.trace", "tmp_out.trace", "tmp_out.dat", 0, false);
    */

    // create cacti config file
    //create_cacti_config("config/test.xml", "dfsfs", "data/cacti_config.cfg");

    /*
    scale_sim_to_memory_trace("dram_filter.csv", "tmp.trace");

    add_data("tmp.trace", "tmp_out.trace", "data/test.dat", 0x989680);
    */

    /*
    while ((opt = getopt(argc, argv, "i:o:e:t:h")) != -1) {
        switch (opt) {
            case 'i':
                input_path = optarg;
                break;
            case 'o':
                output_path = optarg;
                break;
            case 'e':
                coder_name = optarg;
                break;
            case 't':
                trace_path = optarg;
                break;
            case 'h':
                break;
        }
    }

    printf("Running Coding Scheme (%s) \n", coder_name.c_str());
    printf("input  path : %s\n", input_path.c_str());
    printf("output path : %s\n", output_path.c_str());


    // load the input file stream
    std::ifstream in  (input_path);
    std::ofstream out (output_path);
    
    if ( in.is_open() && out.is_open() ) {
    
        // run the encoder
        std::visit([&in=in,&out=out](auto&& arg){ arg.encoder(in,out); }, coder);
        
        // close files
        in.close();
        out.close();
   
        // run analysis
        float avg = mean(output_path,8);
        printf("Bitwise-average = %f\n", avg);

        // create trace file
        streaming_trace(trace_path);

        // create cacti config file
        create_cacti_config("config/test.xml", "dfsfs", "data/cacti_config.cfg");
        
        //ctemplate::TemplateDictionary dict("cacti_config");
        //dict.SetFormattedValue("ACTIVITY", "%.4f", 0.5);
        //std::string cacti_config_out;
        //ctemplate::ExpandTemplate("templates/cacti_config.tpl", 
        //    ctemplate::DO_NOT_STRIP, &dict, &cacti_config_out);
        //std::ofstream cacti_config_path("data/cacti_config.cfg");
        //cacti_config_path << cacti_config_out;
        //cacti_config_path.close();
    } else {
        // TODO: raise error
        fprintf(stderr,"cannot open input or output file \n");
    }
    */

    return 0;
}
