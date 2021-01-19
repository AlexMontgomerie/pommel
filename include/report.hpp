#ifndef REPORT_HPP_ 
#define REPORT_HPP_ 

#include "common.hpp"

#include <nlohmann/json.hpp>

namespace pommel {

class report {
 
typedef struct {
    int burst_size;
    int bitwidth;
    int period;
    float bandwidth;
    float mem_bandwidth;
    float addr_activity;
    float data_activity;
} report_entry_t;
   
    protected:

        std::vector<report_entry_t> entries;

    public:
        
        void add_entry(report_entry_t entry);
        void save_report(std::string report_path);
        analysis();
};

}

#endif
