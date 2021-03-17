#ifndef STREAM_HELPER_HPP_
#define STREAM_HELPER_HPP_

#include "common.hpp"
#include <boost/format.hpp>

enum field_type {ADDR, DIRECTION, DATA}; 

void create_stream(std::istream &addr, std::istream &data, std::string direction, std::ostream &out);
void get_stream_field(std::istream &in, std::ostream &out, field_type field);

uint128_t convert_to_uint128(std::string line);
std::string convert_from_uint128(uint128_t val); 
 
#endif
