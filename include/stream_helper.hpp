#ifndef STREAM_HELPER_HPP_
#define STREAM_HELPER_HPP_

#include "common.hpp"
#include <boost/format.hpp>

enum field_type {ADDR, DIRECTION, DATA}; 

void create_stream(std::istream &addr, std::istream &data, std::string direction, std::ostream &out);
void get_stream_field(std::istream &in, std::ostream &out, field_type field);
void add_stream_field(std::istream &in, std::istream &field_stream, std::ostream &out, field_type field);

#endif
