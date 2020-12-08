#include "stream_helper.hpp"

enum field_type {ADDR, DIRECTION, DATA}; 

void get_stream_field(std::string stream_path, std::ostream out, field_type field) {

    //open stream file
    std::ifstream in (stream_path);
    if(!in.is_open()) {
        throw std::runtime_error("Could not open stream file");
    }

    // open out file
    // std::ostream out (
    
    // iterate over trace file
    std::string line;
    std::string val;
    int addr;
    bool direction;
    int data;
    while( std::getline(in, line) ) {

        // convert line to string stream
        std::stringstream row(line);

        // get address
        std::getline(row,val,' ');
        int addr = stoi(val);

        // get direction
        std::getline(row,val,' ');
        char direction = val.c_str()[0];

        // get data
        std::getline(row,val,' ');
        int data = stoi(val);

        switch(field) {
            case ADDR:
                out << addr << std::endl;
                break;
            case DIRECTION:
                out << direction << std::endl ;
                break;
            case DATA:
                out << data << std::endl;
                break;
            default: ;
        }
    }
}

void get_stream_direction() {

}

void get_stream_data() {

}

void get_stream_addr() {

}
