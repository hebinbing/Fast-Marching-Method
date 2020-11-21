#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "hdf5_parser.hpp"

namespace io{

    H5::PredType get_ds_type(H5::DataSet ds){
        
        if(sizeof(fmm::data_t) == sizeof(int)){
            return H5::PredType::NATIVE_INT;
        } else if(sizeof(fmm::data_t) == sizeof(float)){
            return H5::PredType::NATIVE_FLOAT;
        } else if(sizeof(fmm::data_t) == sizeof(double)){
            return H5::PredType::NATIVE_DOUBLE;
        }
    }

    fmm::function<fmm::data_t> read(std::string const& file_name, std::string const& dataset_name){
        
        H5::H5File file(file_name, H5F_ACC_RDONLY);
        H5::DataSet dataset = file.openDataSet(dataset_name);

        H5::DataSpace dataspace = dataset.getSpace();
        int n_dimensions = dataspace.getSimpleExtentNdims();

        std::vector<hsize_t> dimensions(n_dimensions);
        dataspace.getSimpleExtentDims(dimensions.data());
        
        int n_points = dataspace.getSimpleExtentNpoints();
    
        std::vector<fmm::data_t> ds_values(n_points);
        dataset.read(ds_values.data(), H5::PredType::NATIVE_DOUBLE);

        fmm::function<fmm::data_t> f(ds_values, n_dimensions);

        f.print();

        return f;        
    } 
}