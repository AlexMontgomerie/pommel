#ifndef FEATUREMAP_HPP_ 
#define FEATUREMAP_HPP_ 

#include "common.hpp"
#include <boost/format.hpp>

#include <highfive/H5File.hpp>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>

namespace silence {

class featuremap {
    protected:
        
        //  feautremap file path
        std::string featuremap_path;

        // featuremap dimensions
        //std::vector<std::vector<std::vector<std::vector<int>>>> data;
        std::vector<int> data;
        std::string layer_name;
        //std::vector<int> data;
        unsigned int batch_size;
        unsigned int width;
        unsigned int height;
        unsigned int channels;

        // transforms
        std::vector<int> channel_first_transform(std::vector<int> data_in);

    public:
        
        int get_size(void);
    
        void generate_stream(std::string data_path, std::string transform);
        featuremap(std::string featuremap_path, std::string layer_name);

};

}

#endif
