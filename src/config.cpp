#include "config.hpp"

#include <ctemplate/template.h>
#include "pugixml.hpp"

void create_cacti_config(std::string config_path, std::string stats_path, std::string output_path) {
   
    printf("starting ... \n");
    // create cacti config file
    ctemplate::TemplateDictionary dict("cacti_config");

    // load the DRAM config file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file!");
    }
    
    printf("here ... \n");
 
    // get all relevant parameters    
    std::string dram_type = doc.select_node("/memspec/parameter[@id='memoryType']").node().attribute("value").value();
    int data_width      = doc.select_node("/memspec/memarchitecturespec/parameter[@id='width']").node().attribute("value").as_int();
    int banks           = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfBanks']").node().attribute("value").as_int();
    int ranks           = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfRanks']").node().attribute("value").as_int();
    int cols            = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfColumns']").node().attribute("value").as_int();
    int rows            = doc.select_node("/memspec/memarchitecturespec/parameter[@id='nbrOfRows']").node().attribute("value").as_int();
    int data_rate       = doc.select_node("/memspec/memarchitecturespec/parameter[@id='dataRate']").node().attribute("value").as_int();
    int burst_length    = doc.select_node("/memspec/memarchitecturespec/parameter[@id='burstLength']").node().attribute("value").as_int();
    int clock           = doc.select_node("/memspec/memtimingspec/parameter[@id='clkMhz']").node().attribute("value").as_int();

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
    ctemplate::ExpandTemplate("templates/cacti_config.tpl", 
        ctemplate::DO_NOT_STRIP, &dict, &cacti_config_out);
    std::ofstream cacti_config_path("data/cacti_config.cfg"); // TODO: format with output path
    cacti_config_path << cacti_config_out;
    cacti_config_path.close();

}

