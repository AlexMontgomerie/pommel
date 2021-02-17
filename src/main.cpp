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

using encoder_t = std::variant<
    pommel::encoder<pommel::bi>,
    pommel::encoder<pommel::def>,
    pommel::encoder<pommel::awr>,
    pommel::encoder<pommel::abe>,
    pommel::encoder<pommel::pbm>,
    pommel::encoder<pommel::rle>
>;

using trace_t = std::variant<
    pommel::trace<ramulator::DDR3>,
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
    //} else if( encoder_type == "huffman" ) {    
    //    return pommel::encoder<huffman>(config_path, featuremap, bitwidth);
    } else {
        fprintf(stderr,"ERROR (encoder) : %s not specified!\n", encoder_type.c_str());
    }
}

trace_t get_trace_inst(std::string dram_type, std::string ramulator_config_path, 
        std::string output_path, int burst_size, int period, int bitwidth) {
    if( dram_type == "DDR3" ) { 
        return pommel::trace<ramulator::DDR3>(ramulator_config_path,output_path,burst_size,period,bitwidth);
    }
    if( dram_type == "WIDEIO_SDR" ) { 
        return pommel::trace<ramulator::DDR3>(ramulator_config_path,output_path,burst_size,period,bitwidth);
    }
    if( dram_type == "LPDDR3" ) { 
        return pommel::trace<ramulator::DDR3>(ramulator_config_path,output_path,burst_size,period,bitwidth);
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
    std::string scale_sim_trace_path;
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
        ("scale-sim", "use SCALE-Sim to get bandwidth estimates")
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

        if (vm.count("scale-sim")) {
            use_scale_sim = true;
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
    config_inst.load_network_config(network_config_path);
    config_inst.load_platform_config(accelerator_config_path);

    // create a report
    nlohmann::json report;

    // run scale-sim to get bandwidths
    if ( use_scale_sim ) {
        
        //  create platform config
        std::string scale_sim_config_path   = output_path + "/scale-sim-config.cfg";
        std::string scale_sim_topology_path = output_path + "/scale-sim-topology.csv";
        config_inst.generate_scale_sim_config(scale_sim_config_path, output_path);
        config_inst.generate_scale_sim_topology(featuremap_path, scale_sim_topology_path);

        //  run scale-sim command
        std::string scale_sim_cmd = "python SCALE-Sim/scale.py";
        scale_sim_cmd += " -arch_config=" + scale_sim_config_path; 
        scale_sim_cmd += " -network=" + scale_sim_topology_path; 
        system( scale_sim_cmd.c_str() );

        // add bandwidth 
        config_inst.add_scale_sim_bandwidth(output_path + "/scale-sim-topology_avg_bw.csv");
        
    }

    // get the data packing factor
    config_inst.platform.packing_factor = (int) (config_inst.memory.num_dq/config_inst.platform.bitwidth);

    // iterate over partition for a given accelerator
    for( auto const& partition : config_inst.network ) { 

        // partition information
        std::string partition_index = std::to_string(partition.first);
        pommel::network_config_t partition_conf = partition.second;
      
        // find the actual bandwidth in and out
        float bandwidth_in = std::min( partition_conf.bandwidth_in, config_inst.memory.bandwidth );
        float bandwidth_out = std::min( partition_conf.bandwidth_out, config_inst.memory.bandwidth );

        // get period (clk cycles) for input and output featuremaps
        int period_in   = (int) ( config_inst.platform.burst_size*config_inst.memory.bandwidth / bandwidth_in );
        int period_out  = (int) ( config_inst.platform.burst_size*config_inst.memory.bandwidth / bandwidth_out );

        if ( period_in <= config_inst.platform.burst_size ) {
            printf("WARNING: bandwidth in larger than memory bandwidth\n");
            period_in = config_inst.platform.burst_size+1;
        } 
            
        if ( period_out <= config_inst.platform.burst_size ) {
            printf("WARNING: bandwidth out larger than memory bandwidth\n");
            period_out = config_inst.platform.burst_size+1;
        }

        // TODO: scale memory clock speed to match input bandwidth

        // print debug information
        std::string encoding_scheme_type = get_encoder_type(encoder_config_path);
        printf("Running Partition %s of fpgaconvnet\n", partition_index.c_str() );
        printf("Encoding Scheme: %s\n", encoding_scheme_type.c_str() );
        printf("input featuremap    = %s\n", partition_conf.input_featuremap.c_str()    ); 
        printf("output featuremap   = %s\n", partition_conf.output_featuremap.c_str()   ); 
        printf("---- bitwidth               : %d \n", config_inst.platform.bitwidth ); 
        printf("---- clk freq (MHz)         : %d \n", config_inst.platform.clk_freq ); 
        printf("---- mem bandwidth (GB/s)   : %f \n", config_inst.memory.bandwidth ); 
        printf("---- bandwidth in (GB/s)    : %f \n", partition_conf.bandwidth_in ); 
        printf("---- bandwidth out (GB/s)   : %f \n", partition_conf.bandwidth_out ); 
        printf("---- burst_size             : %d \n", config_inst.platform.burst_size ); 
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
        pommel::featuremap featuremap_input(featuremap_path, partition_conf.input_featuremap);    
        featuremap_input.generate_stream(stream_output_path, config_inst.platform.transform, 
                config_inst.platform.bitwidth, config_inst.platform.packing_factor);

        // get activity and statistics for baseline trace
        pommel::analysis analysis_input_baseline(stream_output_path,config_inst.memory.num_dq,config_inst.memory.addr_width);

        //  encode featuremap
        if(!baseline) {
            encoder_t encoder_input = get_encoder_inst(encoder_config_path, partition_conf.input_featuremap, config_inst.platform);
            std::visit([&stream_in=stream_output_path,&stream_out=encoded_stream_output_path](auto&& arg){
                    arg.encode_stream(stream_in, stream_out); }, encoder_input);
        } else {
            encoded_stream_output_path = stream_output_path;
        }

        // get activity and statistics for trace
        pommel::analysis analysis_input(encoded_stream_output_path,config_inst.memory.num_dq,config_inst.memory.addr_width);

        // generate the trace
        trace_t trace_input = get_trace_inst(config_inst.memory.dram_type, ramulator_config_path, trace_prefix,
                config_inst.platform.burst_size, period_in, config_inst.platform.bitwidth);

        std::visit([&stream_in=encoded_stream_output_path](auto&& arg){
                arg.generate_trace(stream_in); }, trace_input);

        float data_activity_in = analysis_input.get_data_activity();
        float addr_activity_in = analysis_input.get_addr_activity();
        float compression_ratio_in = analysis_input_baseline.get_total_samples()/analysis_input.get_total_samples();
 
        // generate output configs
        config_inst.generate_cacti_config("R",cacti_config_path, data_activity_in, addr_activity_in, 
                (config_inst.platform.burst_size)/((float)period_in) ); 

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
 
        // load the featuremap
        pommel::featuremap featuremap_output(featuremap_path, partition_conf.output_featuremap);    
        featuremap_output.generate_stream(stream_output_path, config_inst.platform.transform,
                config_inst.platform.bitwidth, config_inst.platform.packing_factor);

        // get activity and statistics for baseline trace
        pommel::analysis analysis_output_baseline(stream_output_path,config_inst.memory.num_dq,config_inst.memory.addr_width);

        // encode featuremap
        if(!baseline) {
            encoder_t encoder_output = get_encoder_inst(encoder_config_path, partition_conf.output_featuremap, config_inst.platform);
            std::visit([&stream_in=stream_output_path,&stream_out=encoded_stream_output_path](auto&& arg){
                    arg.encode_stream(stream_in, stream_out); }, encoder_output);
        } else {
            encoded_stream_output_path = stream_output_path;
        }

        // generate the trace
        trace_t trace_output = get_trace_inst(config_inst.memory.dram_type, ramulator_config_path, trace_prefix,
                config_inst.platform.burst_size, period_out, config_inst.platform.bitwidth);
        
        std::visit([&stream_in=encoded_stream_output_path](auto&& arg){
                arg.generate_trace(stream_in); }, trace_output);

        // get activity and statistics for trace
        pommel::analysis analysis_output(encoded_stream_output_path,config_inst.memory.num_dq, config_inst.memory.addr_width);

        float data_activity_out = analysis_output.get_data_activity();
        float addr_activity_out = analysis_output.get_addr_activity();
        float compression_ratio_out = analysis_output_baseline.get_total_samples()/analysis_output.get_total_samples();

        // generate output configs
        config_inst.generate_cacti_config("W", cacti_config_path, data_activity_out, addr_activity_out, 
                (config_inst.platform.burst_size)/((float)period_out) ); 

        // activity information
        printf("---- data activity in       : %f \n", data_activity_in); 
        printf("---- data activity out      : %f \n", data_activity_out); 
        printf("---- address activity in    : %f \n", addr_activity_in); 
        printf("---- address activity out   : %f \n", addr_activity_out); 
        printf("---- compression ratio in   : %f \n", compression_ratio_in); 
        printf("---- compression ratio out  : %f \n", compression_ratio_out); 

        // add report information
        report[partition_index.c_str()]["in"] = { 
            {"bandwidth", bandwidth_in},
            {"data_activity", data_activity_in},
            {"addr_activity", addr_activity_in}
        };
        report[partition_index.c_str()]["out"] = { 
            {"bandwidth", bandwidth_out},
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
