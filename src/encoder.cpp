#include "encoder.hpp"

namespace pommel {

template<>
encoder<def>::encoder(std::string config_path, std::string featuremap, platform_config_t platform) {

    // load config
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file: %s", config_path.c_str());
    }
 
    // get parameters
    std::string featuremap_path = boost::str( boost::format("/encoderspec/layer[@id='%s']") % featuremap );
    pugi::xml_node layer = doc.select_node(featuremap_path.c_str()).node(); 

    // get distance
    int distance = layer.select_node("parameter[@id='distance']").node().attribute("value").as_int();
    
    // initialise the coding scheme
    coder = new def(platform, distance);

}

template<>
encoder<bi>::encoder(std::string config_path, std::string featuremap, platform_config_t platform) {

    // initialise the coding scheme
    coder = new bi(platform);

}

template<>
encoder<awr>::encoder(std::string config_path, std::string featuremap, platform_config_t platform) {

    // initialise the coding scheme
    coder = new awr(platform, 4); // TODO

}

template<>
encoder<abe>::encoder(std::string config_path, std::string featuremap, platform_config_t platform) {

    // initialise the coding scheme
    coder = new abe(platform, 32); //TODO

}

template<>
encoder<pbm>::encoder(std::string config_path, std::string featuremap, platform_config_t platform) {

    // initialise the coding scheme
    coder = new pbm(platform);

}

template<>
encoder<rle>::encoder(std::string config_path, std::string featuremap, platform_config_t platform) {

    // load config
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file: %s", config_path.c_str());
    }
 
    // get parameters
    std::string featuremap_path = boost::str( boost::format("/encoderspec/layer[@id='%s']") % featuremap );
    pugi::xml_node layer = doc.select_node(featuremap_path.c_str()).node(); 

    // get distance
    int rle_zero = layer.select_node("parameter[@id='rle_zero']").node().attribute("value").as_int();
        // initialise the coding scheme
    coder = new rle(platform,rle_zero);

}

}

