#include "config.hpp"

namespace silence {

void config::load_memory_config(std::string config_path) {

    // load the memory config file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file: %s\n", config_path.c_str());
    }
 
    // get all memory parameters 
    memory_config.dram_type       = doc.select_node("/memspec/parameter[@id='memoryType']").node().attribute("value").value();
    memory_config.data_width      = doc.select_node("/memspec/memarchitecturespec/parameter[@id='width']").node().attribute("value").as_int();
    memory_config.banks           = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfBanks']").node().attribute("value").as_int();
    memory_config.rank            = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfRanks']").node().attribute("value").as_int();
    memory_config.cols            = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfColumns']").node().attribute("value").as_int();
    memory_config.rows            = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfRows']").node().attribute("value").as_int();
    memory_config.data_rate       = doc.select_node("/memspec/memarchitecturespec/parameter[@id='dataRate']").node().attribute("value").as_int();
    memory_config.burst_length    = doc.select_node("/memspec/memarchitecturespec/parameter[@id='burstLength']").node().attribute("value").as_int();
    memory_config.clock           = doc.select_node("/memspec/memtimingspec/parameter[@id='clkMhz']").node().attribute("value").as_int();

    return;
}


void config::load_accelerator_config(std::string config_path) {
    
    // load the memory config file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file: %s\n", config_path.c_str());
    }

    // get accelerator name
    pugi::xml_node root = doc.select_node("/acceleratorspec").node();
    std::string accelerator_name = root.attribute("type").value();

    // iterate over partitions
    for( pugi::xml_node partition = root.first_child(); partition; partition = partition.next_sibling() ) {
       
        // get partition index
        int partition_index = partition.attribute("id").as_int();

        // get accelerator configuration
        accelerator_config_t conf;
        conf.bitwidth   = partition.select_node("parameter[@id='bitwidth']").node().attribute("value").as_int();
        conf.burst_size = partition.select_node("parameter[@id='burst_size']").node().attribute("value").as_int();
        conf.period     = partition.select_node("parameter[@id='period']").node().attribute("value").as_int();
        conf.transform  = partition.select_node("parameter[@id='transform']").node().attribute("value").value();
        conf.input_featuremap   = partition.select_node("parameter[@id='input_featuremap']").node().attribute("value").value();
        conf.output_featuremap  = partition.select_node("parameter[@id='output_featuremap']").node().attribute("value").value();
        
        // update accelerator configuration
        accelerator_config[partition_index] = conf;
    }

}

void config::generate_ramulator_config(std::string config_path) { 

    // create ramulator config file
    ctemplate::TemplateDictionary dict("ramulator_config");

    // add config
    dict.SetValue("STANDARD", memory_config.dram_type);
    dict.SetValue("CHANNELS", "1");
    dict.SetFormattedValue("RANKS", "%d", memory_config.rank);
    dict.SetValue("SPEED", "DDR3_1600K"); // FIXME 
    dict.SetValue("ORG", "DDR3_2Gb_x8"); // FIXME

    // save to file
    std::string config_out;
    ctemplate::ExpandTemplate("templates/ramulator_template.tpl", 
        ctemplate::DO_NOT_STRIP, &dict, &config_out);
    std::ofstream ramulator_config_path(config_path);
    ramulator_config_path << config_out;
    ramulator_config_path.close();

}

void config::generate_cacti_config(std::string config_path) {

    // create cacti config file
    ctemplate::TemplateDictionary dict("cacti_config");

    // convert DRAMPower config to cacti config
    // DRAM TYPE
    if ( memory_config.dram_type == "DDR3" ) {
        dict.SetValue("DRAM_TYPE", "D");
    } else if ( memory_config.dram_type == "LPDDR2" ) {
        dict.SetValue("DRAM_TYPE", "L");
    } else if ( memory_config.dram_type == "WIDEIO_SDR" ) {
        dict.SetValue("DRAM_TYPE", "W");
    } else {
        // TODO: raise error
    }

    // IO STATE
    dict.SetValue("IO_STATE", "W");
    
    // ADDR TIMING
    dict.SetFormattedValue("ADDR_TIMING", "%.4f", (float) (1/(float) memory_config.data_rate) );

    // BUS BW
    dict.SetFormattedValue("BUS_BW", "%d", (int) 2*(memory_config.clock/1000)*memory_config.data_width );
    
    // MEM DENSITY 
    dict.SetFormattedValue("MEM_DENSITY", "%.4f", 0.5); // TODO

    // BUS FREQ
    dict.SetFormattedValue("BUS_FREQ", "%d", memory_config.clock); 
    
    // DUTY CYCLE
    dict.SetFormattedValue("DUTY_CYCLE", "%.4f", 1.0); 
    
    // ACTIVITY DQ
    dict.SetFormattedValue("ACTIVITY_DQ", "%.4f", 0.5); 
   
    // ACTIVITY CA 
    dict.SetFormattedValue("ACTIVITY_CA", "%.4f", 0.5); 
    
    // NUM DQ
    dict.SetFormattedValue("NUM_DQ", "%d", memory_config.data_width); 
 
    // NUM DQS
    dict.SetFormattedValue("NUM_DQS", "%d", memory_config.data_width); 
 
    // NUM CA
    dict.SetFormattedValue("NUM_CA", "%d", memory_config.data_width); 
 
    // NUM CLK
    dict.SetFormattedValue("NUM_CLK", "%d", 2); 
    
    // NUM MEM DQ
    dict.SetFormattedValue("NUM_MEM_DQ", "%d", memory_config.rank); 
 
    // MEM DATA WIDTH
    dict.SetFormattedValue("MEM_DATA_WIDTH", "%d", memory_config.data_width); 
    
    std::string cacti_config_out;
    ctemplate::ExpandTemplate("templates/cacti_template.tpl", 
        ctemplate::DO_NOT_STRIP, &dict, &cacti_config_out);
    std::ofstream cacti_config_path(config_path); // TODO: format with output path
    cacti_config_path << cacti_config_out;
    cacti_config_path.close();

}

void config::generate_dram_power_config(std::string config_path) {

}

}

