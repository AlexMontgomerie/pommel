#include "common.hpp"
#include <ctemplate/template.h>

#include "coding_scheme.hpp"
#include "coding_schemes/def.hpp"
#include "coding_schemes/abe.hpp"
#include "coding_schemes/pbm.hpp"
#include "coding_schemes/bi.hpp"
#include "coding_schemes/awr.hpp"
#include "coding_schemes/rle.hpp"

#include "config.hpp"
#include "analysis.hpp"
#include "trace.hpp"
#include "convert.hpp"

using coder_t = std::variant<def,abe,pbm,bi,awr,rle>;

coder_t get_coder(std::string coder_name) {

    if (coder_name == "def") {
        return def(8,1);
    } else if (coder_name == "abe") {
        return abe(8,32);
    } else if (coder_name == "bi") {
        return bi(8);
    } else if (coder_name == "awr") {
        return awr(8,4);
    } else if (coder_name == "rle") {
        return rle(8,0);
    }
}

int main(int argc, char *argv[]) {

    // parameters
    int opt;
    std::string input_path;
    std::string output_path;
    std::string trace_path;
    std::string coder_name;
    
    scale_sim_to_memory_trace("dram_ifmap.csv", "tmp.trace");

    /*
    while ((opt = getopt(argc, argv, "i:o:e:t:h")) != -1) {
        switch (opt) {
            case 'i':
                input_path = optarg;
                break;
            case 'o':
                output_path = optarg;
                break;
            case 'e':
                coder_name = optarg;
                break;
            case 't':
                trace_path = optarg;
                break;
            case 'h':
                break;
        }
    }

    printf("Running Coding Scheme (%s) \n", coder_name.c_str());
    printf("input  path : %s\n", input_path.c_str());
    printf("output path : %s\n", output_path.c_str());

    auto coder = get_coder(coder_name);

    // load the input file stream
    std::ifstream in  (input_path);
    std::ofstream out (output_path);
    
    if ( in.is_open() && out.is_open() ) {
    
        // run the encoder
        std::visit([&in=in,&out=out](auto&& arg){ arg.encoder(in,out); }, coder);
        
        // close files
        in.close();
        out.close();
   
        // run analysis
        float avg = mean(output_path,8);
        printf("Bitwise-average = %f\n", avg);

        // create trace file
        streaming_trace(trace_path);

        // create cacti config file
        create_cacti_config("config/test.xml", "dfsfs", "data/cacti_config.cfg");
        
        //ctemplate::TemplateDictionary dict("cacti_config");
        //dict.SetFormattedValue("ACTIVITY", "%.4f", 0.5);
        //std::string cacti_config_out;
        //ctemplate::ExpandTemplate("templates/cacti_config.tpl", 
        //    ctemplate::DO_NOT_STRIP, &dict, &cacti_config_out);
        //std::ofstream cacti_config_path("data/cacti_config.cfg");
        //cacti_config_path << cacti_config_out;
        //cacti_config_path.close();
    } else {
        // TODO: raise error
        fprintf(stderr,"cannot open input or output file \n");
    }
    */

    return 0;
}
