#include "trace.hpp"

std::string get_line(std::fstream& file, unsigned int line) {
    file.seekg(std::ios::beg);
    for(int i=0; i < line - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    std::string out;
    file >> out;
    return out;

}

void add_data(std::string trace_in_file_path, std::string trace_out_file_path, std::string datafile_path, int addr_offset, bool use_addr) {

    // open traceinfile
    std::ifstream trace_in_file(trace_in_file_path);
    if(!trace_in_file.is_open()) throw std::runtime_error("Could not open traceinfile!");

    // open traceoutfile
    std::ofstream trace_out_file(trace_out_file_path);
    if(!trace_out_file.is_open()) throw std::runtime_error("Could not open traceoutfile!");

    // open datafile
    std::fstream datafile(datafile_path);
    if(!datafile.is_open()) throw std::runtime_error("Could not open datafile!");

    // iterate over lines in the file in
    std::string line;
    std::string val;
    int index = 0;
    while( std::getline(trace_in_file,line) ) {
        
        index++;
        
        // convert line to string stream
        std::stringstream row(line);
        
        // get the addr 
        std::getline(row, val, ' ');
        int addr = std::stoul(val, nullptr, 16);

        // get the direction
        std::getline(row, val, ' ');
        char direction = (char) val[0];

        // get data
        int data;
        if ( use_addr ) {
            data = stoi( get_line(datafile, addr-addr_offset + 1 ) );
        } else {
            data = stoi( get_line(datafile, index ) );
        }

        // save to outfile
        char buffer[50];
        sprintf(buffer, "0x%X %c %d\n", addr, direction, data);
        trace_out_file << buffer; 

    }

    // close files
    trace_in_file.close();
    trace_out_file.close();
    datafile.close();
}

void get_data(std::string tracefile_path, std::string datafile_path) {


    // open tracefile
    std::ifstream tracefile(tracefile_path);
    if(!tracefile.is_open()) throw std::runtime_error("Could not open trace file!");

    // open datafile
    std::ofstream datafile(datafile_path);
    if(!datafile.is_open()) throw std::runtime_error("Could not open datafile!");

    // iterate over lines in the file in
    std::string line;
    std::string val;
    while( std::getline(tracefile,line) ) {
        
        // convert line to string stream
        std::stringstream row(line);
        
        // get the addr 
        std::getline(row, val, ' ');
        int addr = std::stoul(val, nullptr, 16);

        // get the direction
        std::getline(row, val, ' ');
        char direction = (char) val[0];

        // get data
        try {
            std::getline(row, val, ' ');
            int data = stoi(val);
    
            // save to data file
            char buffer[50];
            sprintf(buffer, "%d\n", data);
            datafile << buffer; 

        } 
        catch( std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
        }

    }

    // close files
    tracefile.close();
    datafile.close();

}

//void generate_trace(std::ofstream tracefile, unsigned int burst_size, unsigned int nop_cycles,

/*
void streaming_trace(std::string tracefile_path, int fm_size, int burst_size, int bitwidth) {

    // create trace
    int in_addr  = 0;
    int out_addr = fm_size;
    std::ofstream tracefile(tracefile_path);
    for(int i=0; i<(int)(fm_size/burst_size); i++) {
        // input burst
        for( int j=0;j<burst_size;j++ ) {
            tracefile << boost::format("0x%X R") % in_addr << std::endl;
            in_addr += (int) (bitwidth/8);
        }
        // output burst
        for( int j=0;j<burst_size;j++ ) {
            tracefile << boost::format("0x%X W") % out_addr << std::endl;
            out_addr += (int) (bitwidth/8); 
        }
    }    
}


void processing_engine_trace(std::string tracefile_path, fm_dim_t fm_in, int filters=128, int bitwidth=8) {

    // get the output dimensions
    int rows_out = ;
    int cols_out = ;

    // initialise base addresses
    int in_base_addr = 0;
    int out_base_addr = fm_in.rows*fm_in.cols*fm_in.channels;

    // tracefile setup
    std::ofstream tracefile(tracefile_path);
    
    // iterate over dimensions out
    for(int i=0;i<rows_out;i++) {
        for(int j=0;j<cols_out;j++) {
            // read input
            for(int k=0;k<fm_in.channels;k++) {
                for(int k1=0;k1<fm_in.kernel_size;k1++) {
                    for(int k2=0;k2<fm_in.kernel_size;k2++) {
                        int in_addr = in_base_addr + ( i * fm_in.cols * fm_in.channels +
                            j * fm_in.channels + 
                            k) + ( k1 * fm_in.kernel_size + k2 );
                    }
                }
            }
            // write output
            for(int k=0;k<filters;k++) {
                int out_addr = out_base_addr + ( i * cols_out * filters +
                        j * filters +
                        k) * ( (int) (bitwidth/8) );
                tracefile << std::format("0x%X W", out_addr) << std::endl;
            }
        }
    }

}
*/
