#include "featuremap.hpp"

namespace pommel {

int featuremap::get_size(void) {
    return batch_size*channels*height*width;
}

/**
 * Transforms
 */

std::vector<int32_t> featuremap::channel_major_transform(std::vector<int32_t> data_in) {
    
    std::vector<int32_t> data_out;

    for(int i=0;i<batch_size;i++) {
        for(int j=0;j<height;j++) {
            for(int k=0;k<width;k++) {
                for(int l=0;l<channels;l++) {
                    
                    int index = i*channels*height*width +
                        j*width +
                        k + 
                        l*height*width;

                    data_out.push_back(data_in[index]);
                }
            }
        }
    }

    return data_out;
}

std::vector<int32_t> featuremap::row_major_transform(std::vector<int32_t> data_in) {
    
    std::vector<int32_t> data_out;

    for(int i=0;i<batch_size;i++) {
        for(int l=0;l<channels;l++) {
            for(int j=0;j<height;j++) {
                for(int k=0;k<width;k++) {
                    int index = i*channels*height*width +
                        j*width +
                        k + 
                        l*height*width;

                    data_out.push_back(data_in[index]);
                }
            }
        }
    }

    return data_out;
}

void featuremap::generate_stream(std::string data_path, std::string transform, int bitwidth, int data_packing_factor, std::string direction) {

    // get transformed data 
    std::vector<int32_t> transformed_data;
    if(transform == "channel-major") {
        transformed_data = channel_major_transform(data);
    } else if(transform == "row-major") {
        transformed_data = row_major_transform(data);
    }

    // get mask for bitwidth
    uint32_t bitmask = (1<<bitwidth)-1;

    // save to a .dat file
    std::ofstream datafile(data_path);
    int i=0, j=0;
    uint128_t val = 0;
    for(auto const& value: transformed_data) {
        val |= ( (uint128_t) (value & bitmask) ) << bitwidth*j;
        j = (j+1) % data_packing_factor;
        if(j==0) {
            std::string data = convert_from_uint128(val);
            datafile << boost::format("%i %s %s\n") % i % direction % data;
            i++;
            val = 0;
        }
    }
    datafile.close();
}

featuremap::featuremap(std::string featuremap_path, std::string layer_name) : featuremap_path(featuremap_path), layer_name(layer_name) {

    // load h5 file
    HighFive::File file(featuremap_path, HighFive::File::ReadOnly );

    // get the data
    HighFive::DataSet layer = file.getDataSet(layer_name);

    // get the dimensions of the layer
    std::vector<size_t> dim = layer.getDimensions();
    
    // store dimensions of layer
    if ( dim.size() == 4 ) {
        batch_size  = dim[0];
        channels    = dim[1];
        width       = dim[2];
        height      = dim[3];
    } else if (dim.size() == 2) {
        batch_size  = dim[0];
        channels    = dim[1];
        width       = 1;
        height      = 1;
    } else {
        batch_size  = dim[0];
        channels    = 1;
        width       = 1;
        height      = 1;
    }

    // get featuremap
    featuremap_size = get_size();
    int32_t* tmp = new int32_t[featuremap_size];
    layer.read(tmp);

    // convert to vector
    data.insert(data.begin(), &tmp[0], &tmp[featuremap_size-1]);

    return;
}

}
