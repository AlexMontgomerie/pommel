#include "common.hpp"

#include "coding_scheme.hpp"
#include "coding_schemes/def.hpp"
#include "coding_schemes/abe.hpp"
#include "coding_schemes/pbm.hpp"
#include "coding_schemes/bi.hpp"
#include "coding_schemes/awr.hpp"
#include "coding_schemes/rle.hpp"

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
    std::string coder_name;
    

    while ((opt = getopt(argc, argv, "i:o:e:h")) != -1) {
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
    
    } else {
        // TODO: raise error
    }

    return 0;
}
