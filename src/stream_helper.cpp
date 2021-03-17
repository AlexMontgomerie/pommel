#include "stream_helper.hpp"

uint128_t convert_to_uint128(std::string line) {
    // value
    uint128_t val;
    std::string tmp;

    // convert line to string stream
    std::stringstream row(line);

    // get first half data
    std::getline(row,tmp,' ');
    val = std::stoull(tmp);

    // get second half data
    std::getline(row,tmp,' ');
    return val | ( ( (uint128_t) std::stoull(tmp) ) << 64 );

}

std::string convert_from_uint128(uint128_t val) {
    
    // value
    uint64_t data[2];

    data[0] = val;
    data[1] = val >> 64;

    //printf("here %ld %ld \n", data[0], data[1]);
    return boost::str( boost::format("%ld %ld ") % data[0] % data[1] );

}

void create_stream(std::istream &addr, std::istream &data, std::string direction, std::ostream &out) {

    // iterate over trace file
    std::string line;
    std::string addr_val;
    std::string data_val;
    int i=0;
    while( std::getline(data, data_val) ) {
        out << i << " " << direction << " " << data_val << " " << std::endl;
        i++;
    }
}

void get_stream_field(std::istream &in, std::ostream &out, field_type field) {

    // iterate over trace file
    std::string line;
    std::string val;
    uint64_t addr;
    bool direction;
    uint64_t data[2];
    while( std::getline(in, line) ) {

        // convert line to string stream
        std::stringstream row(line);

        // get address
        std::getline(row,val,' ');
        addr = std::stoull(val);

        // get direction
        std::getline(row,val,' ');
        direction = val.c_str()[0];

        // get data
        std::getline(row,val,' ');
        data[0] = std::stoull(val);
        std::getline(row,val,' ');
        data[1] = std::stoull(val);

        switch(field) {
            case ADDR:
                out << addr << std::endl;
                break;
            case DIRECTION:
                out << direction << std::endl ;
                break;
            case DATA:
                out << data[0] << " " << data[1] << " " << std::endl;
                break;
            default: ;
        }
    }
}

