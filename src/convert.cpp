#include "convert.hpp"


void scale_sim_to_memory_trace(std::string infile_path, std::string outfile_path, char direction) {
   
    // open file in
    std::ifstream infile(infile_path);
    if(!infile.is_open()) throw std::runtime_error("Could not open file in!");

    // open file out
    std::ofstream outfile(outfile_path);
    if(!outfile.is_open()) throw std::runtime_error("Could not open file out!");

    // iterate over lines in the file in
    std::string line;
    std::string val;
    while( std::getline(infile,line) ) {
        
        // convert line to string stream
        std::stringstream row(line);
        
        // get the timing
        std::getline(row, val, ',');
        int timing = stoi(val);
        
        // get the addresses
        while( std::getline(row,val,',') ) {

            try {
            
                // convert to address
                int addr = (int) stof(val);
            
                // save to outfile
                char buffer[50];
                sprintf(buffer, "0x%X %c\n", addr, direction);
                outfile << buffer; 

            } catch (const std::invalid_argument& e) {
                break;
            }

        }

    }

}
