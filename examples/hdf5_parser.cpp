#include <iostream>
#include <string>
#include <vector>

#include "hdf5_parser.hpp"

namespace io{

    // size_t

    void read(std::string const& file_name, std::string const& dataset_name){
        
        H5::H5File file(file_name, H5F_ACC_RDONLY);
        H5::DataSet dataset = file.openDataSet(dataset_name);

        H5::DataSpace dataspace = dataset.getSpace();
        int num_dimensions = dataspace.getSimpleExtentNdims();
    
        std::vector<hsize_t> dimensions(num_dimensions);
        dataspace.getSimpleExtentDims(dimensions.data());
        
        int n_points = dataspace.getSimpleExtentNpoints();

        fmm::function<float> f(n_points, num_dimensions, n_points);

    }   
}