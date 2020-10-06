#include "analysis.hpp"

std::vector<float> bitwise_mean(std::istream &in, unsigned int bitwidth) {

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

uint32_t mean(std::istream &in, unsigned int bitwidth) {

}

uint32_t variance(std::istream &in, unsigned int bitwidth) {

}

uint32_t transitions(std::istream &in, unsigned int bitwidth) {

}

uint32_t switching_activity_mean(std::istream &in, unsigned int bitwidth) {

}

uint32_t switching_activity_variance(std::istream &in, unsigned int bitwidth) {

}
