#include "featuremap.hpp"

#include <boost/numeric/ublas/matrix.hpp>

namespace silence {

int featuremap::get_size(void) {
    return batch_size*channels*height*width;
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
