#ifndef FEATUREMAP_HPP_ 
#define FEATUREMAP_HPP_ 

#include "common.hpp"
#include <boost/format.hpp>

#include <highfive/H5File.hpp>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>

namespace pommel {

class featuremap {
    protected:

        int MAX_SIZE = 10000;
        int featuremap_size;

        //  feautremap file path
        std::string featuremap_path;

        // featuremap dimensions
        std::vector<uint64_t> data;
        std::string layer_name;
        
        // transforms
        std::vector<uint64_t> row_major_transform(std::vector<uint64_t> data_in);
        std::vector<uint64_t> channel_major_transform(std::vector<uint64_t> data_in);

    public:
 
        unsigned int batch_size;
        unsigned int width;
        unsigned int height;
        unsigned int channels;

       
        int get_size(void);
    
        void generate_stream(std::string data_path, std::string transform, 
                int bitwidth, int data_packing_factor);
        featuremap(std::string featuremap_path, std::string layer_name);

};

}

#endif
