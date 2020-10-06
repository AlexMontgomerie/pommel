#ifndef ANALYSIS_HPP_ 
#define ANALYSIS_HPP_ 

#include "common.hpp"

std::vector<float> bitwise_mean(std::istream &in, unsigned int bitwidth);
std::vector<uint32_t> bitwise_variance(std::istream &in, unsigned int bitwidth);
std::vector<uint32_t> bitwise_transitions(std::istream &in, unsigned int bitwidth);
std::vector<uint32_t> bitwise_switching_activity_mean(std::istream &in, unsigned int bitwidth);
std::vector<uint32_t> bitwise_switching_activity_variance(std::istream &in, unsigned int bitwidth);

uint32_t mean(std::istream &in, unsigned int bitwidth);
uint32_t variance(std::istream &in, unsigned int bitwidth);
uint32_t transitions(std::istream &in, unsigned int bitwidth);
uint32_t switching_activity_mean(std::istream &in, unsigned int bitwidth);
uint32_t switching_activity_variance(std::istream &in, unsigned int bitwidth);

#endif
