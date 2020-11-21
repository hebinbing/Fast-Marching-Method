#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "hdf5_parser.hpp"

namespace io{

    H5::PredType get_ds_type(H5::DataSet ds){
        
        switch (ds.getTypeClass()){
            
            case H5T_INTEGER:
                return H5::PredType::NATIVE_INT;
            case H5T_FLOAT:
                return H5::PredType::NATIVE_FLOAT;
            default:
                break;
        }
    }

    fmm::function<float> read(std::string const& file_name, std::string const& dataset_name){
        
        H5::H5File file(file_name, H5F_ACC_RDONLY);
        H5::DataSet dataset = file.openDataSet(dataset_name);

        H5::DataSpace dataspace = dataset.getSpace();
        int n_dimensions = dataspace.getSimpleExtentNdims();

        std::vector<hsize_t> dimensions(n_dimensions);
        dataspace.getSimpleExtentDims(dimensions.data());
        
        int n_points = dataspace.getSimpleExtentNpoints();

        int dim_size = pow(n_points, 1.0/n_dimensions);
    
        std::vector<float> ds_values(n_points, 0.0);
        dataset.read(ds_values.data(), get_ds_type(dataset));

        fmm::function<float> f(ds_values, n_dimensions);
        
        return f;        
    } 
}