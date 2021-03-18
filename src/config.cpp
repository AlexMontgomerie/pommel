#include "config.hpp"

namespace pommel {

void config::load_memory_config(std::string config_path) {

    // load the memory config file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file: %s\n", config_path.c_str());
    }
 
    // get all memory parameters 
    memory.dram_type       = doc.select_node("/memspec/parameter[@id='memoryType']").node().attribute("value").value();
    memory.data_width      = doc.select_node("/memspec/memarchitecturespec/parameter[@id='width']").node().attribute("value").as_int();
    memory.num_chips       = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfChips']").node().attribute("value").as_int();
    memory.banks           = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfBanks']").node().attribute("value").as_int();
    memory.rank            = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfRanks']").node().attribute("value").as_int();
    memory.cols            = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfColumns']").node().attribute("value").as_int();
    memory.rows            = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfRows']").node().attribute("value").as_int();
    memory.data_rate       = doc.select_node("/memspec/memarchitecturespec/parameter[@id='dataRate']").node().attribute("value").as_int();
    memory.burst_length    = doc.select_node("/memspec/memarchitecturespec/parameter[@id='burstLength']").node().attribute("value").as_int();
    memory.clock           = doc.select_node("/memspec/memtimingspec/parameter[@id='clkMhz']").node().attribute("value").as_int();

    // get capacity
    memory.capacity = (int) (memory.banks*memory.rank*memory.cols*memory.rows/memory.data_width);
 
    // get number of pins
    memory.addr_width = (int) log2( (float) memory.capacity );
    memory.num_dq     = (int) ( (memory.num_chips*memory.data_width) );// /memory.rank );
   
    // get memory bandwidth
    memory.bandwidth = ((memory.data_rate*memory.clock*memory.num_dq)/(8*1000.0)); 
    
    return;
}

void config::load_network_config(std::string config_path) {
    
    // load the memory config file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file: %s\n", config_path.c_str());
    }

    // get accelerator name
    pugi::xml_node root = doc.select_node("/networkspec").node();
    std::string network_name = root.attribute("name").value();

    // iterate over partitions
    for( pugi::xml_node partition = root.first_child(); partition; partition = partition.next_sibling() ) {
       
        // get partition index
        int partition_index = partition.attribute("id").as_int();

        // get accelerator configuration
        network_config_t conf;
        conf.kernel_size        = partition.select_node("parameter[@id='kernel_size']").node().attribute("value").as_int();
        conf.stride             = partition.select_node("parameter[@id='stride']").node().attribute("value").as_int();
        conf.input_featuremap   = partition.select_node("parameter[@id='input_featuremap']").node().attribute("value").value();
        conf.output_featuremap  = partition.select_node("parameter[@id='output_featuremap']").node().attribute("value").value();
        
        // load bandwidth if it exists
        if( partition.select_node("parameter[@id='bandwidth_in']") ) {
            conf.bandwidth_in   = partition.select_node("parameter[@id='bandwidth_in']").node().attribute("value").as_float();
        }
        if( partition.select_node("parameter[@id='bandwidth_out']") ) { 
            conf.bandwidth_out  = partition.select_node("parameter[@id='bandwidth_out']").node().attribute("value").as_float();
        }

        // update accelerator configuration
        network[partition_index] = conf;
    }

}

void config::load_platform_config(std::string config_path) {
    
    // load the accelerator config file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file: %s\n", config_path.c_str());
    }
 
    // get all platform parameters 
    platform.dataflow        = doc.select_node("/acceleratorspec/parameter[@id='dataflow']").node().attribute("value").value();
    platform.array_height    = doc.select_node("/acceleratorspec/parameter[@id='array_height']").node().attribute("value").as_int();
    platform.array_width     = doc.select_node("/acceleratorspec/parameter[@id='array_width']").node().attribute("value").as_int();
    platform.ifmap_sram_size = doc.select_node("/acceleratorspec/parameter[@id='ifmap_sram_size']").node().attribute("value").as_int();
    platform.filter_sram_size= doc.select_node("/acceleratorspec/parameter[@id='filter_sram_size']").node().attribute("value").as_int();
    platform.ofmap_sram_size = doc.select_node("/acceleratorspec/parameter[@id='ofmap_sram_size']").node().attribute("value").as_int();
    platform.clk_freq        = doc.select_node("/acceleratorspec/parameter[@id='clk_freq']").node().attribute("value").as_int();
    platform.burst_size      = doc.select_node("/acceleratorspec/parameter[@id='burst_size']").node().attribute("value").as_int();
    platform.bitwidth        = doc.select_node("/acceleratorspec/parameter[@id='bitwidth']").node().attribute("value").as_int();
    platform.transform       = doc.select_node("/acceleratorspec/parameter[@id='transform']").node().attribute("value").value();
    
    return;

}

void config::generate_ramulator_config(std::string config_path) { 

    // create ramulator config file
    ctemplate::TemplateDictionary dict("ramulator_config");

    // add config
    dict.SetValue("STANDARD", memory.dram_type);
    dict.SetValue("CHANNELS", "1");
    dict.SetFormattedValue("RANKS", "%d", memory.rank);
    dict.SetFormattedValue("SPEED", "%s_%dK",memory.dram_type.c_str(), memory.clock);
    dict.SetFormattedValue("ORG", "%s_%dGb_x%d", memory.dram_type.c_str(),ceil(memory.capacity/8589934592), memory.data_width);

    // save to file
    std::string config_out;
    ctemplate::ExpandTemplate("templates/ramulator_template.tpl", 
        ctemplate::DO_NOT_STRIP, &dict, &config_out);
    std::ofstream ramulator_config_path(config_path);
    ramulator_config_path << config_out;
    ramulator_config_path.close();

}

void config::generate_cacti_config(std::string direction, std::string config_path, float data_activity, float address_activity, float duty_cycle) {

    // create cacti config file
    ctemplate::TemplateDictionary dict("cacti_config");

    // convert DRAMPower config to cacti config
    // DRAM TYPE
    if ( memory.dram_type == "DDR3" ) {
        dict.SetValue("DRAM_TYPE", "DDR3");
        dict.SetValue("ADDR_TIMING", "1.0"); 
    } else if ( memory.dram_type == "DDR3L" ) {
        dict.SetValue("DRAM_TYPE", "DDR3L");
        dict.SetValue("ADDR_TIMING", "1.0"); 
    } else if ( memory.dram_type == "DDR4" ) {
        dict.SetValue("DRAM_TYPE", "DDR4");
        dict.SetValue("ADDR_TIMING", "1.0"); 
    } else if ( memory.dram_type == "LPDDR2" ) {
        dict.SetValue("DRAM_TYPE", "LPDDR2");
        dict.SetValue("ADDR_TIMING", "0.5"); 
    } else if ( memory.dram_type == "LPDDR3" ) {
        dict.SetValue("DRAM_TYPE", "LPDDR2");
        dict.SetValue("ADDR_TIMING", "0.5"); 
    } else if ( memory.dram_type == "WIDEIO_SDR" ) {
        dict.SetValue("DRAM_TYPE", "WideIO");
        dict.SetValue("ADDR_TIMING", "1.0"); 
    } else {
        // TODO: raise error
    }

    dict.SetValue("IO_STATE", direction); 
    dict.SetFormattedValue("BUS_BW", "%.4f", memory.bandwidth);
    dict.SetFormattedValue("MEM_DENSITY", "%d", memory.capacity/(1000000*memory.banks*memory.rank)); 
    dict.SetFormattedValue("BUS_FREQ", "%d", memory.clock); 
    dict.SetFormattedValue("DUTY_CYCLE", "%.4f", duty_cycle); 
    dict.SetFormattedValue("ACTIVITY_DQ", "%.4f", data_activity);
    dict.SetFormattedValue("ACTIVITY_CA", "%.4f", address_activity);
    dict.SetFormattedValue("NUM_DQ", "%d", memory.num_dq); 
    dict.SetFormattedValue("NUM_DQS", "%d", (int)(2*memory.num_dq/8)); // maybe x2 ?
    dict.SetFormattedValue("NUM_CA", "%d", memory.addr_width);
    dict.SetFormattedValue("NUM_CLK", "%d", 2);
    dict.SetFormattedValue("NUM_MEM_DQ", "%d", memory.rank); 
    dict.SetFormattedValue("MEM_DATA_WIDTH", "%d", memory.data_width);
    
    std::string cacti_config_out;
    ctemplate::ExpandTemplate("templates/cacti_template.tpl", 
        ctemplate::DO_NOT_STRIP, &dict, &cacti_config_out);
    std::ofstream cacti_config_path(config_path);
    cacti_config_path << cacti_config_out;
    cacti_config_path.close();

}

}

