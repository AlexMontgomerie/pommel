#include "featuremap.hpp"

#include <boost/numeric/ublas/matrix.hpp>

namespace silence {

int featuremap::get_size(void) {
    return batch_size*channels*height*width;
}

/**
 * Transforms
 */

std::vector<int> featuremap::channel_first_transform(std::vector<int> data_in) {
    
    std::vector<int> data_out;

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

void featuremap::generate_stream(std::string data_path, std::string transform) {

    // get transformed data 
    std::vector<int> transformed_data;
    if(transform == "channel-first") {
        transformed_data = channel_first_transform(data);
    }

    // save to a .dat file
    std::ofstream datafile(data_path);
    for(auto const& value: transformed_data) {
        //datafile << boost::format("%i\n") % ( value & ( (1 << 8) - 1 ) );
        datafile << boost::format("%i\n") % value;
    }
    datafile.close();
}

featuremap::featuremap(std::string featuremap_path, std::string layer_name) : featuremap_path(featuremap_path), layer_name(layer_name) {

    // load h5 file
    HighFive::File file("data/test.h5", HighFive::File::ReadOnly );

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
    } else {
        batch_size  = dim[0];
        channels    = dim[1];
        width       = 1;
        height      = 1;
    }

    // get featuremap
    int tmp [get_size()];
    layer.read(tmp);

    // convert to vector
    data.insert(data.begin(), &tmp[0], &tmp[get_size()-1]);
   
    return;
}

}
