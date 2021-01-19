#include "stream_helper.hpp"

void get_stream_field(std::istream &in, std::ostream &out, field_type field) {

    // iterate over trace file
    std::string line;
    std::string val;
    uint64_t addr;
    bool direction;
    uint64_t data;
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
        data = std::stoull(val);

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

void add_stream_field(std::istream &in, std::istream &field_stream, std::ostream &out, field_type field) {

    // iterate over trace file
    std::string line;
    std::string val;
    uint64_t addr;
    bool direction;
    uint64_t data;
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
        data = std::stoull(val);

        // get the field data
        if( !(field_stream >> val) ) {
            // TODO: raise error
        }
        
        // write to output stream
        switch(field) {
            case ADDR:
                out << boost::format("%s %c %i\n") % val.c_str() % direction % data;
                break;
            case DIRECTION:
                out << boost::format("%i %s %i\n") % addr % val.c_str() % data;
                break;
            case DATA:
                out << boost::format("%i %c %s\n") % addr % direction % val.c_str();
                break;
            default: ;
        }
    }
}


