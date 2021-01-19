#include "encoder.hpp"

namespace pommel {

template<>
encoder<def>::encoder(std::string config_path, std::string featuremap, int bitwidth) {

    // load config
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file: %s", config_path.c_str());
    }
 
    // get parameters
    std::string featuremap_path = boost::str( boost::format("/encoderspec/layer[@id='%s']") % featuremap );
    pugi::xml_node layer = doc.select_node(featuremap_path.c_str()).node(); 

    // get parameters
    int distance = layer.select_node("parameter[@id='distance']").node().attribute("value").as_int();
    
    // initialise the coding scheme
    coder = new def(bitwidth, distance);

}

template<>
encoder<bi>::encoder(std::string config_path, std::string featuremap, int bitwidth) {

    // initialise the coding scheme
    coder = new bi(bitwidth);

}

template<>
encoder<awr>::encoder(std::string config_path, std::string featuremap, int bitwidth) {

    // initialise the coding scheme
    coder = new awr(bitwidth, 4);

}

template<>
encoder<abe>::encoder(std::string config_path, std::string featuremap, int bitwidth) {

    // initialise the coding scheme
    coder = new abe(bitwidth, 32);

}

template<>
encoder<pbm>::encoder(std::string config_path, std::string featuremap, int bitwidth) {

    // initialise the coding scheme
    coder = new pbm(bitwidth);

}

}

