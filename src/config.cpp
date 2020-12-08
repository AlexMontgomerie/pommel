#include "config.hpp"

namespace silence {

void config::load_memory_config(std::string config_path) {

    // load the memory config file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file!");
    }
 
    // get all memory parameters 
    dram_type       = doc.select_node("/memspec/parameter[@id='memoryType']").node().attribute("value").value();
    data_width      = doc.select_node("/memspec/memarchitecturespec/parameter[@id='width']").node().attribute("value").as_int();
    banks           = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfBanks']").node().attribute("value").as_int();
    rank            = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfRanks']").node().attribute("value").as_int();
    cols            = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfColumns']").node().attribute("value").as_int();
    rows            = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfRows']").node().attribute("value").as_int();
    data_rate       = doc.select_node("/memspec/memarchitecturespec/parameter[@id='dataRate']").node().attribute("value").as_int();
    burst_length    = doc.select_node("/memspec/memarchitecturespec/parameter[@id='burstLength']").node().attribute("value").as_int();
    clock           = doc.select_node("/memspec/memtimingspec/parameter[@id='clkMhz']").node().attribute("value").as_int();

    return;
}

void config::load_platform_config(std::string config_path) {

}

void config::generate_ramulator_config(std::string config_path) { 

    // create ramulator config file
    ctemplate::TemplateDictionary dict("ramulator_config");

    // add config
    dict.SetValue("STANDARD", dram_type);
    dict.SetValue("CHANNELS", "1");
    dict.SetFormattedValue("RANKS", "%d", rank);
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
    if ( dram_type == "DDR3" ) {
        dict.SetValue("DRAM_TYPE", "D");
    } else if ( dram_type == "LPDDR2" ) {
        dict.SetValue("DRAM_TYPE", "L");
    } else if ( dram_type == "WIDEIO_SDR" ) {
        dict.SetValue("DRAM_TYPE", "W");
    } else {
        // TODO: raise error
    }

    // IO STATE
    dict.SetValue("IO_STATE", "W");
    
    // ADDR TIMING
    dict.SetFormattedValue("ADDR_TIMING", "%.4f", (float) (1/(float) data_rate) );

    // BUS BW
    dict.SetFormattedValue("BUS_BW", "%d", (int) 2*(clock/1000)*data_width );
    
    // MEM DENSITY 
    dict.SetFormattedValue("MEM_DENSITY", "%.4f", 0.5); // TODO

    // BUS FREQ
    dict.SetFormattedValue("BUS_FREQ", "%d", clock); 
    
    // DUTY CYCLE
    dict.SetFormattedValue("DUTY_CYCLE", "%.4f", 1.0); 
    
    // ACTIVITY DQ
    dict.SetFormattedValue("ACTIVITY_DQ", "%.4f", 0.5); 
   
    // ACTIVITY CA 
    dict.SetFormattedValue("ACTIVITY_CA", "%.4f", 0.5); 
    
    // NUM DQ
    dict.SetFormattedValue("NUM_DQ", "%d", data_width); 
 
    // NUM DQS
    dict.SetFormattedValue("NUM_DQS", "%d", data_width); 
 
    // NUM CA
    dict.SetFormattedValue("NUM_CA", "%d", data_width); 
 
    // NUM CLK
    dict.SetFormattedValue("NUM_CLK", "%d", 2); 
    
    // NUM MEM DQ
    dict.SetFormattedValue("NUM_MEM_DQ", "%d", rank); 
 
    // MEM DATA WIDTH
    dict.SetFormattedValue("MEM_DATA_WIDTH", "%d", data_width); 
    
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

