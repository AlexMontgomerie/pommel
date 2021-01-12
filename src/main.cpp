#include "common.hpp"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
using namespace boost::program_options;

#include "config.hpp"
#include "analysis.hpp"
#include "trace.hpp"
#include "featuremap.hpp"
#include "encoder.hpp"

#include "coding_schemes/def.hpp"
#include "coding_schemes/abe.hpp"
#include "coding_schemes/pbm.hpp"
#include "coding_schemes/bi.hpp"
#include "coding_schemes/awr.hpp"
#include "coding_schemes/rle.hpp"

using encoder_t = std::variant<
    silence::encoder<bi>,
    silence::encoder<def>,
    silence::encoder<awr>,
    silence::encoder<abe>,
    silence::encoder<pbm>,
    silence::encoder<rle>
>;

using trace_t = std::variant<
    silence::trace<ramulator::DDR3>
>;

std::string get_encoder_type(std::string config_path) {
    // load the memory config file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file: %s\n", config_path.c_str());
    }
    // return encoder name
    return doc.select_node("/encoderspec").node().attribute("type").value();
 }

encoder_t get_encoder_inst(std::string config_path, std::string featuremap, int bitwidth) {

    // get the encoder type
    std::string encoder_type = get_encoder_type(config_path);

    // get encoder
    if( encoder_type == "bi" ) {    
        return silence::encoder<bi>(config_path, featuremap, bitwidth);
    } else if( encoder_type == "def" ) {    
        return silence::encoder<def>(config_path, featuremap, bitwidth);
    } else if( encoder_type == "awr" ) {    
        return silence::encoder<awr>(config_path, featuremap, bitwidth);
    } else if( encoder_type == "abe" ) {    
        return silence::encoder<abe>(config_path, featuremap, bitwidth);
    } else if( encoder_type == "pbm" ) {    
        return silence::encoder<pbm>(config_path, featuremap, bitwidth);
    } else if( encoder_type == "rle" ) {    
        return silence::encoder<def>(config_path, featuremap, bitwidth);
    //} else if( encoder_type == "huffman" ) {    
    //    return silence::encoder<huffman>(config_path, featuremap, bitwidth);
    } else {
        fprintf(stderr,"ERROR (encoder) : %s not specified!\n", encoder_type.c_str());
    }
}

trace_t get_trace_inst(std::string dram_type, std::string ramulator_config_path, 
        std::string output_path, int burst_size, int period, int bitwidth) {
    if( dram_type == "DDR3" ) { 
        return silence::trace<ramulator::DDR3>(ramulator_config_path,output_path,burst_size,period,bitwidth);
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
        std::string partition_index = std::to_string(partition.first);
        silence::accelerator_config_t partition_conf = partition.second;
       
        // get period (clk cycles) for input and output featuremaps
        int period_in = (int) (partition_conf.burst_size*partition_conf.clk_freq)/(partition_conf.bandwidth_in*1000);
        int period_out = (int) (partition_conf.burst_size*partition_conf.clk_freq)/(partition_conf.bandwidth_out*1000);

        // print debug information
        printf("Running Partition %s of fpgaconvnet\n", partition_index.c_str() );
        printf("input featuremap    = %s\n", partition_conf.input_featuremap.c_str()    ); 
        printf("output featuremap   = %s\n", partition_conf.output_featuremap.c_str()   ); 
        printf("---- bitwidth               : %d \n", partition_conf.bitwidth ); 
        printf("---- clk freq (MHz)         : %d \n", partition_conf.clk_freq ); 
        printf("---- bandwidth in (Gbps)    : %f \n", partition_conf.bandwidth_in ); 
        printf("---- bandwidth out (Gbps)   : %f \n", partition_conf.bandwidth_out ); 
        printf("---- burst_size             : %d \n", partition_conf.burst_size ); 
        printf("---- period in              : %d \n", period_in ); 
        printf("---- period out             : %d \n", period_out ); 
 
        // create output paths
        boost::filesystem::create_directory(output_path);
        boost::filesystem::create_directory(output_path+"/"+partition_index);
        std::string stream_output_path;
        std::string encoded_stream_output_path;
        std::string ramulator_config_path;
        std::string cacti_config_path;
        std::string trace_prefix;

        /**
         * Input Featuremap
         */

        stream_output_path          = output_path + "/" + partition_index + "/input_stream.dat";
        encoded_stream_output_path  = output_path + "/" + partition_index + "/input_stream_encoded.dat";
        ramulator_config_path       = output_path + "/" + partition_index + "/input_ramulator.cfg";
        cacti_config_path           = output_path + "/" + partition_index + "/input_cacti.cfg";
        trace_prefix                = output_path + "/" + partition_index + "/input-";

        // generate config for ramulator and trace
        config_inst.generate_ramulator_config(ramulator_config_path); 
 
        //  load the featuremap
        silence::featuremap featuremap_input(featuremap_path, partition_conf.input_featuremap);    
        featuremap_input.generate_stream(stream_output_path, partition_conf.transform);

        //  encode featuremap
        encoder_t encoder_input = get_encoder_inst(encoder_config_path, partition_conf.input_featuremap, partition_conf.bitwidth);
        std::visit([&stream_in=stream_output_path,&stream_out=encoded_stream_output_path](auto&& arg){
                arg.encode_stream(stream_in, stream_out); }, encoder_input);

        // generate the trace
        trace_t trace_input = get_trace_inst(config_inst.memory_config.dram_type, ramulator_config_path, trace_prefix,
                partition_conf.burst_size, period_in, partition_conf.bitwidth);

        std::visit([&stream_in=encoded_stream_output_path](auto&& arg){
                arg.generate_trace(stream_in); }, trace_input);

        // get activity and statistics for trace
        silence::analysis analysis_input(stream_output_path,config_inst.memory_config.data_width,config_inst.memory_config.addr_width);

        // generate output configs
        config_inst.generate_cacti_config(cacti_config_path, partition_conf.bandwidth_in, 
                analysis_input.get_data_activity(), analysis_input.get_addr_activity()); 

        /**
         * Output Featuremap
         */

        stream_output_path          = output_path + "/" + partition_index + "/output_stream.dat";
        encoded_stream_output_path  = output_path + "/" + partition_index + "/output_stream_encoded.dat";
        ramulator_config_path       = output_path + "/" + partition_index + "/output_ramulator.cfg";
        cacti_config_path           = output_path + "/" + partition_index + "/output_cacti.cfg";
        trace_prefix                = output_path + "/" + partition_index + "/output-";

        // generate config for ramulator and trace
        config_inst.generate_ramulator_config(ramulator_config_path); 
 
        //  load the featuremap
        silence::featuremap featuremap_output(featuremap_path, partition_conf.output_featuremap);    
        featuremap_output.generate_stream(stream_output_path, partition_conf.transform);

        //  encode featuremap
        encoder_t encoder_output = get_encoder_inst(encoder_config_path, partition_conf.output_featuremap, partition_conf.bitwidth);
        std::visit([&stream_in=stream_output_path,&stream_out=encoded_stream_output_path](auto&& arg){
                arg.encode_stream(stream_in, stream_out); }, encoder_output);

        // generate the trace
        trace_t trace_output = get_trace_inst(config_inst.memory_config.dram_type, ramulator_config_path, trace_prefix,
                partition_conf.burst_size, period_out, partition_conf.bitwidth);
        
        std::visit([&stream_in=encoded_stream_output_path](auto&& arg){
                arg.generate_trace(stream_in); }, trace_output);

        // get activity and statistics for trace
        silence::analysis analysis_output(stream_output_path,config_inst.memory_config.data_width,config_inst.memory_config.addr_width);

        // generate output configs
        config_inst.generate_cacti_config(cacti_config_path, partition_conf.bandwidth_out, 
                analysis_output.get_data_activity(), analysis_output.get_addr_activity()); 

    }

    // end execution
    return 0;
}
