#include "common.hpp"

#include "nlohmann/json.hpp"

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
#include "coding_schemes/huffman.hpp"

using encoder_t = std::variant<
    pommel::encoder<pommel::bi>,
    pommel::encoder<pommel::def>,
    pommel::encoder<pommel::awr>,
    pommel::encoder<pommel::abe>,
    pommel::encoder<pommel::pbm>,
    pommel::encoder<pommel::rle>,
    pommel::encoder<pommel::huffman>
>;

using trace_t = std::variant<
    pommel::trace<ramulator::DDR3>,
    pommel::trace<ramulator::DDR4>,
    pommel::trace<ramulator::WideIO>,
    pommel::trace<ramulator::LPDDR3>
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

encoder_t get_encoder_inst(std::string config_path, std::string featuremap, pommel::platform_config_t platform) {

    // get the encoder type
    std::string encoder_type = get_encoder_type(config_path);

    // get encoder
    if( encoder_type == "bi" ) {
        return pommel::encoder<pommel::bi>(config_path, featuremap, platform);
    } else if( encoder_type == "def" ) {    
        return pommel::encoder<pommel::def>(config_path, featuremap, platform);
    } else if( encoder_type == "awr" ) {    
        return pommel::encoder<pommel::awr>(config_path, featuremap, platform);
    } else if( encoder_type == "abe" ) {    
        return pommel::encoder<pommel::abe>(config_path, featuremap, platform);
    } else if( encoder_type == "pbm" ) {    
        return pommel::encoder<pommel::pbm>(config_path, featuremap, platform);
    } else if( encoder_type == "rle" ) {    
        return pommel::encoder<pommel::rle>(config_path, featuremap, platform);
    } else if( encoder_type == "huffman" ) {    
        return pommel::encoder<pommel::huffman>(config_path, featuremap, platform);
    } else {
        fprintf(stderr,"ERROR (encoder) : %s not specified!\n", encoder_type.c_str());
    }
}

trace_t get_trace_inst(std::string dram_type, std::string ramulator_config_path, 
        std::string output_path, int burst_size, int period, int bitwidth) {
    if( dram_type == "DDR3" ) { 
        return pommel::trace<ramulator::DDR3>(ramulator_config_path,output_path,burst_size,period,bitwidth);
    }
    else if( dram_type == "DDR3L" ) { 
        return pommel::trace<ramulator::DDR3>(ramulator_config_path,output_path,burst_size,period,bitwidth);
    }
    else if( dram_type == "DDR4" ) { 
        return pommel::trace<ramulator::DDR4>(ramulator_config_path,output_path,burst_size,period,bitwidth);
    }
    else if( dram_type == "WIDEIO_SDR" ) { 
        //return pommel::trace<ramulator::WideIO>(ramulator_config_path,output_path,burst_size,period,bitwidth);
        return pommel::trace<ramulator::DDR3>(ramulator_config_path,output_path,burst_size,period,bitwidth);
    }
    else if( dram_type == "LPDDR3" ) { 
        return pommel::trace<ramulator::LPDDR3>(ramulator_config_path,output_path,burst_size,period,bitwidth);
    }
    else {
        fprintf(stderr,"ERROR (trace) : %s not specified!\n", dram_type.c_str());
    }
}

int main(int argc, char *argv[]) {

    // parameters
    int opt;
    
    // config file paths
    std::string memory_config_path;
    std::string encoder_config_path;
    std::string accelerator_config_path;
    std::string network_config_path;

    // encoding scheme
    std::string coder_name;

    // file path names
    std::string featuremap_path;
    std::string trace_path;
    std::string data_path;
    std::string output_path;

    // flags
    bool baseline = false;
    bool use_scale_sim = false;

    try {
 
        // add command line options
        options_description desc("Allowed Options");
        desc.add_options()
        ("help,h", "help message")
        ("baseline", "compute baseline power readings (no encoding)")
        ("memory", value(&memory_config_path)->required(), "file path for memory config (.xml)")
        ("encoder", value(&encoder_config_path), "file path for encoding scheme config (.xml)")
        ("network", value(&network_config_path)->required(), "file path for network config (.xml)")
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

        if (vm.count("baseline")) {
            baseline = true;
        }

        notify(vm);

    }
    catch(std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 0;
    }

    // create configs
    pommel::config config_inst;

    // load configs
    config_inst.load_memory_config(memory_config_path);
    std::string network_name = config_inst.load_network_config(network_config_path);
    config_inst.load_platform_config(accelerator_config_path);

    // create a report
    nlohmann::json report;

    // add all paths
    report["memory_path"]       = memory_config_path.c_str();
    report["network_path"]      = network_config_path.c_str();
    report["accelerator_path"]  = accelerator_config_path.c_str();
    report["encoder_path"]      = encoder_config_path.c_str();

    //get types 
    report["dram_type"] = config_inst.memory.dram_type.c_str();
    report["accelerator_name"] = config_inst.platform.name.c_str();
    report["network_name"] = network_name.c_str();
    report["encoder_type"] = coder_name.c_str();

    // get the data packing factor
    config_inst.platform.packing_factor = (int) (config_inst.memory.num_dq/config_inst.platform.bitwidth);

    // TODO: add some information at the start

    // iterate over partition for a given accelerator
    for( auto const& partition : config_inst.network ) { 

        // partition information
        std::string partition_index = std::to_string(partition.first);
        pommel::network_config_t partition_conf = partition.second;
      

        // create output paths
        boost::filesystem::create_directory(output_path);
        boost::filesystem::create_directory(output_path+"/"+partition_index);
        
        // temporary ramulator config path
        std::string ramulator_config_path;
        
        /**
         * Input Featuremap
         */

        std::string stream_in_path = output_path + "/" + partition_index + "/input_stream.dat";
        std::string encoded_stream_in_path = output_path + "/" + partition_index + "/input_stream_encoded.dat";
        ramulator_config_path = output_path + "/" + partition_index + "/input_ramulator.cfg";

        // generate config for ramulator and trace
        config_inst.generate_ramulator_config(ramulator_config_path); 
 
        //  load the featuremap
        pommel::featuremap featuremap_input(featuremap_path, partition_conf.input_featuremap);    
        featuremap_input.generate_stream(stream_in_path, config_inst.platform.transform, 
                config_inst.platform.bitwidth, config_inst.platform.packing_factor, "R");

        // get activity and statistics for baseline trace
        pommel::analysis analysis_input_baseline(stream_in_path, config_inst.memory.num_dq, config_inst.memory.addr_width);

        //  encode featuremap
        if(!baseline) {
            encoder_t encoder_input = get_encoder_inst(encoder_config_path, partition_conf.input_featuremap, config_inst.platform);
            std::visit([&stream_in=stream_in_path,&stream_out=encoded_stream_in_path](auto&& arg){
                    arg.encode_stream(stream_in, stream_out, "R"); }, encoder_input);
        } else {
            encoded_stream_in_path = stream_in_path;
        }

        // get activity and statistics for trace
        pommel::analysis analysis_input(encoded_stream_in_path,config_inst.memory.num_dq,config_inst.memory.addr_width);
            
        // get readings
        float data_activity_in = analysis_input.get_data_activity();
        float addr_activity_in = analysis_input.get_addr_activity();
        float compression_ratio_in = analysis_input_baseline.get_total_samples()/analysis_input.get_total_samples();
        float total_samples_in = analysis_input.get_total_samples();
        
        /**
         * Output Featuremap
         */

        std::string stream_out_path = output_path + "/" + partition_index + "/output_stream.dat";
        std::string encoded_stream_out_path = output_path + "/" + partition_index + "/output_stream_encoded.dat";
        ramulator_config_path = output_path + "/" + partition_index + "/output_ramulator.cfg";

        // generate config for ramulator and trace
        config_inst.generate_ramulator_config(ramulator_config_path); 
 
        // load the featuremap
        pommel::featuremap featuremap_output(featuremap_path, partition_conf.output_featuremap);    
        featuremap_output.generate_stream(stream_out_path, config_inst.platform.transform,
                config_inst.platform.bitwidth, config_inst.platform.packing_factor, "W");

        // get activity and statistics for baseline trace
        pommel::analysis analysis_output_baseline(stream_out_path,config_inst.memory.num_dq,config_inst.memory.addr_width);

        // encode featuremap
        if(!baseline) {
            encoder_t encoder_output = get_encoder_inst(encoder_config_path, partition_conf.output_featuremap, config_inst.platform);
            std::visit([&stream_in=stream_out_path,&stream_out=encoded_stream_out_path](auto&& arg){
                    arg.encode_stream(stream_in, stream_out, "W"); }, encoder_output);
        } else {
            encoded_stream_out_path = stream_out_path;
        }

        // get activity and statistics for trace
        pommel::analysis analysis_output(encoded_stream_out_path,config_inst.memory.num_dq, config_inst.memory.addr_width);

        // get readings
        float data_activity_out = analysis_output.get_data_activity();
        float addr_activity_out = analysis_output.get_addr_activity();
        float compression_ratio_out = analysis_output_baseline.get_total_samples()/analysis_output.get_total_samples();
        float total_samples_out = analysis_output.get_total_samples();
        
        // output paths for external tools
        std::string cacti_read_config_path      = output_path + "/" + partition_index + "/read_cacti.cfg";
        std::string cacti_write_config_path     = output_path + "/" + partition_index + "/write_cacti.cfg";
        std::string cacti_idle_config_path      = output_path + "/" + partition_index + "/idle_cacti.cfg";
        std::string trace_prefix                = output_path + "/" + partition_index + "/trace-";

        // find the actual bandwidth in and out
        float bandwidth_in  = partition_conf.bandwidth_in / compression_ratio_in;
        float bandwidth_out = partition_conf.bandwidth_out / compression_ratio_out;

        // get period (clk cycles) for input and output featuremaps
        int period_in   = (int) ( config_inst.platform.burst_size*config_inst.memory.bandwidth / bandwidth_in );
        int period_out  = (int) ( config_inst.platform.burst_size*config_inst.memory.bandwidth / bandwidth_out );

        // calculate burst transfer period
        uint32_t period = std::max(period_in, period_out);
        if ( period <= 2*config_inst.platform.burst_size ) {
            printf("WARNING: required bandwidth larger than memory bandwidth\n");
            period = 2*config_inst.platform.burst_size+1;
        }

        // actual bandwidth
        float bandwidth = ( 2*config_inst.platform.burst_size*config_inst.memory.bandwidth ) /( (float) period );

        // generate combined trace
        trace_t trace = get_trace_inst(config_inst.memory.dram_type, ramulator_config_path, trace_prefix,
                config_inst.platform.burst_size, period, config_inst.platform.bitwidth);
        
        std::visit([&stream_in=encoded_stream_in_path,&stream_out=encoded_stream_out_path](auto&& arg){
                arg.generate_combined_trace(stream_in, stream_out); }, trace);

        // calculate read, write and idle duty cycles
        float read_duty_cycle  = config_inst.platform.burst_size / ((float) period);
        float write_duty_cycle = config_inst.platform.burst_size / ((float) period);
        float idle_duty_cycle  = 1 - (read_duty_cycle+write_duty_cycle);

        // generate the cacti config files
        config_inst.generate_cacti_config("READ",cacti_read_config_path, data_activity_in, 
                addr_activity_in, read_duty_cycle);

        config_inst.generate_cacti_config("WRITE",cacti_write_config_path, data_activity_out, 
                addr_activity_out, write_duty_cycle);

        config_inst.generate_cacti_config("IDLE",cacti_idle_config_path, 0.0, 
                0.0, idle_duty_cycle);

        // print debug information
        std::string encoding_scheme_type = get_encoder_type(encoder_config_path);
        printf("Running Partition %s of %s \n", partition_index.c_str(), output_path.c_str() );
        printf("Encoding Scheme: %s\n", encoding_scheme_type.c_str() );
        printf("input featuremap    = %s\n", partition_conf.input_featuremap.c_str()    ); 
        printf("output featuremap   = %s\n", partition_conf.output_featuremap.c_str()   ); 
        printf("---- bitwidth               : %d \n", config_inst.platform.bitwidth ); 
        printf("---- clk freq (MHz)         : %d \n", config_inst.platform.clk_freq ); 
        printf("---- mem bandwidth (GB/s)   : %f \n", config_inst.memory.bandwidth ); 
        printf("---- bandwidth (GB/s)       : %f \n", bandwidth ); 
        printf("---- burst_size             : %d \n", config_inst.platform.burst_size ); 
        printf("---- period                 : %d \n", period ); 
        printf("---- read duty cycle        : %f \n", read_duty_cycle ); 
        printf("---- write duty cycle       : %f \n", write_duty_cycle ); 
        printf("---- idle duty cycle        : %f \n", idle_duty_cycle ); 
        printf("---- data activity in       : %f \n", data_activity_in); 
        printf("---- data activity out      : %f \n", data_activity_out); 
        printf("---- address activity in    : %f \n", addr_activity_in); 
        printf("---- address activity out   : %f \n", addr_activity_out); 
        printf("---- compression ratio in   : %f \n", compression_ratio_in); 
        printf("---- compression ratio out  : %f \n", compression_ratio_out); 

        // add report information
        report["partitions"][partition_index.c_str()] = { 
            {"read_duty_cycle" , read_duty_cycle},
            {"write_duty_cycle", write_duty_cycle},
            {"idle_duty_cycle" , idle_duty_cycle}
        };
        report["partitions"][partition_index.c_str()]["in"] = { 
            {"bandwidth", bandwidth_in},
            {"samples", total_samples_in},
            {"compression_ratio", compression_ratio_in},
            {"data_activity", data_activity_in},
            {"addr_activity", addr_activity_in}
        };
        report["partitions"][partition_index.c_str()]["out"] = { 
            {"bandwidth", bandwidth_out},
            {"samples", total_samples_out},
            {"compression_ratio", compression_ratio_out},
            {"data_activity", data_activity_out},
            {"addr_activity", addr_activity_out}
        };
    }

    // save report
    std::ofstream report_file(output_path+"/"+"report.json");
    report_file << report.dump(4);
    report_file.close();

    // end execution
    return 0;
}
