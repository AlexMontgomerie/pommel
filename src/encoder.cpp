#include "encoder.hpp"

namespace silence {

template<>
encoder<def>::encoder(std::string config_path) {

    // load config
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file!");
    }
 
    // get parameters
    int bitwidth = doc.select_node("/encoderspec/bitwidth").node().attribute("value").as_int();
    int distance = doc.select_node("/encoderspec/distance").node().attribute("value").as_int();

    // initialise the coding scheme
    coder = new def(bitwidth, distance);

}

template<>
encoder<bi>::encoder(std::string config_path) {

    /*
    // load config
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file!");
    }
 
    // get parameters
    int bitwidth = doc.select_node("/encoderspec/bitwidth").node().attribute("value").as_int();
    int distance = doc.select_node("/encoderspec/distance").node().attribute("value").as_int();
    */

    // initialise the coding scheme
    coder = new bi(8);

}
}

