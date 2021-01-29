#include "analysis.hpp"

namespace pommel {

float analysis::get_addr_activity(void) {
    return analysis::get_total_transitions(addr_stream)/(addr_width*addr_stream.size());
}

float analysis::get_data_activity(void) {
    return analysis::get_total_transitions(data_stream)/(data_width*2*data_stream.size());
}

float analysis::get_total_transitions(std::vector<uint64_t> stream) {
    
    // iterate over vector
    uint64_t transitions = 0;
    uint64_t value_prev = 0;
    for(auto const& value: stream) {
        transitions += __builtin_popcountl(value^value_prev);
        value_prev = value;
    }
    
    // return total transistions
    return transitions;

}

analysis::analysis(std::string trace_path, int data_width, int addr_width) : data_width(data_width), addr_width(addr_width) {

    std::string line;
    
    // open trace file
    std::ifstream trace (trace_path);
    if( !trace.is_open() ) {
        fprintf(stderr,"cannot open input file: %s \n", trace_path.c_str());
    }

    // get address stream
    std::stringstream addr_stream_raw;
    get_stream_field(trace, addr_stream_raw, ADDR);

    // convert stream to a vector
    while( std::getline(addr_stream_raw, line) ) {
        addr_stream.push_back(std::stoull(line));
    }

    // set to beginning
    trace.clear();
    trace.seekg(0, std::ios::beg);

    // get address stream
    std::stringstream data_stream_raw;
    get_stream_field(trace, data_stream_raw, DATA);

    // convert stream to a vector
    while( std::getline(data_stream_raw, line) ) {
        data_stream.push_back(std::stoull(line));
    }

    // close file
    trace.close();
}

}
