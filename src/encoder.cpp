#include "encoder.hpp"

namespace pommel {

template<>
encoder<def>::encoder(std::string config_path, std::string featuremap, platform_config_t platform) : platform(platform) {

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
encoder<bi>::encoder(std::string config_path, std::string featuremap, platform_config_t platform) : platform(platform) {

    // initialise the coding scheme
    coder = new bi(platform);

}

template<>
encoder<awr>::encoder(std::string config_path, std::string featuremap, platform_config_t platform) : platform(platform) {

    // initialise the coding scheme
    coder = new awr(platform, 4); // TODO

}

template<>
encoder<abe>::encoder(std::string config_path, std::string featuremap, platform_config_t platform) : platform(platform) {

    // initialise the coding scheme
    coder = new abe(platform, 32); //TODO

}

template<>
encoder<pbm>::encoder(std::string config_path, std::string featuremap, platform_config_t platform) : platform(platform) {

    // load config
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file: %s", config_path.c_str());
    }
    
    // get parameters
    std::string featuremap_path = boost::str( boost::format("/encoderspec/layer[@id='%s']") % featuremap );
    pugi::xml_node layer = doc.select_node(featuremap_path.c_str()).node(); 
 
    // value mask
    uint32_t mask = (1 << platform.bitwidth) - 1;

    // load the code table
    std::map<uint32_t,uint32_t> code_table;
    pugi::xml_node code_table_xml = layer.select_node("code_table").node(); 
    for( const auto& child : code_table_xml.children() ) {
        uint32_t key = child.attribute("key").as_int() & mask;
        uint32_t value = child.attribute("value").as_int() & mask;
        code_table.insert(std::make_pair(key,value));
    }    

    // initialise the coding scheme
    coder = new pbm(platform, code_table);

}

template<>
encoder<rle>::encoder(std::string config_path, std::string featuremap, platform_config_t platform) : platform(platform) {

    // load config
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file: %s", config_path.c_str());
    }
 
    // get parameters
    std::string featuremap_path = boost::str( boost::format("/encoderspec/layer[@id='%s']") % featuremap );
    pugi::xml_node layer = doc.select_node(featuremap_path.c_str()).node(); 

    // get rle zero
    uint32_t rle_zero = layer.select_node("parameter[@id='rle_zero']").node().attribute("value").as_int();

    // get batch_size
    uint32_t batch_size = layer.select_node("parameter[@id='batch_size']").node().attribute("value").as_int();

    // initialise the coding scheme
    coder = new rle(platform, rle_zero, batch_size);

}

template<>
encoder<huffman>::encoder(std::string config_path, std::string featuremap, platform_config_t platform) : platform(platform) {

    // load config
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) {
        fprintf(stderr,"Couldn't open config file: %s", config_path.c_str());
    }
    
    // get parameters
    std::string featuremap_path = boost::str( boost::format("/encoderspec/layer[@id='%s']") % featuremap );
    pugi::xml_node layer = doc.select_node(featuremap_path.c_str()).node(); 
 
    // value mask
    uint32_t mask = (1 << platform.bitwidth) - 1;

    // load the code table
    std::map<uint32_t,uint32_t> code_table;
    pugi::xml_node code_table_xml = layer.select_node("code_table").node(); 
    for( const auto& child : code_table_xml.children() ) {
        uint32_t key = child.attribute("key").as_int() & mask;
        uint32_t value = child.attribute("value").as_int() & mask;
        code_table.insert(std::make_pair(key,value));
    }    

    // get batch_size
    uint32_t batch_size = layer.select_node("parameter[@id='batch_size']").node().attribute("value").as_int();

    // initialise the coding scheme
    coder = new huffman(platform, code_table, batch_size);

}

}

