#include "analysis.hpp"

namespace silence {

float analysis::get_addr_activity(void) {
    return analysis::get_total_transitions(addr_stream)/(addr_width*addr_stream.size());
}

float analysis::get_data_activity(void) {
    return analysis::get_total_transitions(data_stream)/(data_width*data_stream.size());
}

float analysis::get_total_transitions(std::vector<uint32_t> stream) {
    
    // iterate over vector
    uint32_t transitions = 0;
    uint32_t value_prev = 0;
    for(auto const& value: stream) {
        transitions += __builtin_popcount(value^value_prev);
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
        addr_stream.push_back(stoi(line));
    }

    // set to beginning
    trace.clear();
    trace.seekg(0, std::ios::beg);

    // get address stream
    std::stringstream data_stream_raw;
    get_stream_field(trace, data_stream_raw, DATA);

    // convert stream to a vector
    while( std::getline(data_stream_raw, line) ) {
        data_stream.push_back(stoi(line));
    }

}

}

std::vector<uint32_t> get_address_trace(std::string trace_path) {
    
    // open trace file

    // iterate over trace


}

std::vector<float> bitwise_mean(std::string infile_path, unsigned int bitwidth) {

    // bitwise mean
    float acc_cache[bitwidth];
    
    // initialise acc_cache
    for(int i=0; i<bitwidth; i++) {
        acc_cache[i] = 0.0;
    }

    // value buffer
    uint32_t val;

    size_t stream_length;

    // iterate over the rest of the stream
    std::ifstream in(infile_path);
    while (in >> val) {
        for(int i=0; i<bitwidth; i++) {
            acc_cache[i] += (float) ( ( val >> i ) & 0x1 );
        }
        stream_length++;
    }

    // get bitwise mean vector
    std::vector<float> out;
    for(int i=0; i<bitwidth; i++) {
        out.push_back(acc_cache[i]/(float)stream_length);
    }

    // close file
    in.close();

    // return the vector
    return out;
}

std::vector<uint32_t> bitwise_variance(std::istream &in, unsigned int bitwidth) {

}

std::vector<uint32_t> bitwise_transitions(std::istream &in, unsigned int bitwidth) {

}

std::vector<uint32_t> bitwise_switching_activity_mean(std::istream &in, unsigned int bitwidth) {

}

std::vector<uint32_t> bitwise_switching_activity_variance(std::istream &in, unsigned int bitwidth) {

}

float mean(std::string infile_path, unsigned int bitwidth) {

    // get bitwise-mean
    std::vector<float> bw_mean = bitwise_mean(infile_path, bitwidth);

    float avg = 0.0;
    for(int i=0;i<bitwidth;i++) {
        avg += bw_mean[i]/bitwidth;
    }

    return avg;
}

uint32_t variance(std::istream &in, unsigned int bitwidth) {

}

uint32_t transitions(std::istream &in, unsigned int bitwidth) {

}

uint32_t switching_activity_mean(std::istream &in, unsigned int bitwidth) {

}

uint32_t switching_activity_variance(std::istream &in, unsigned int bitwidth) {

}
