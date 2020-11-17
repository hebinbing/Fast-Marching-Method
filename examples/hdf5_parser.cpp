#include <iostream>
#include <string>
#include <vector>

#include "hdf5_parser.hpp"

namespace io{
    
    void read(std::string const& file_name, std::string const& dataset_name){
        
        H5::H5File file(file_name, H5F_ACC_RDONLY);
        H5::DataSet dataset = file.openDataSet(dataset_name);

        H5T_class_t type_class = dataset.getTypeClass();
        if (type_class != H5T_FLOAT){
            std::cerr << "Unable to read dataset, incorrect dataset type class" << std::endl;
            return;
        }
        
        H5::FloatType datatype = dataset.getFloatType();
        size_t precision = datatype.getSize();
        if (precision != sizeof(double)){
            std::cerr << "Unable to read dataset, incorrect dataset type precision" << std::endl;
            return;
        }
        
        H5::DataSpace dataspace = dataset.getSpace();
        int num_dimensions = dataspace.getSimpleExtentNdims();
    
        std::vector<hsize_t> dimensions(num_dimensions);
        int dataset_size = dataspace.getSimpleExtentDims(dimensions.data());
       
        std::cout << "num_dimensions " << num_dimensions << ", dimensions " << (unsigned long)(dimensions[0]) << " x " << (unsigned long)(dimensions[1]) << std::endl;
        std::cout << "ds size = " << dataset_size << std::endl;
        
        std::vector<hsize_t> memspace_dim(num_dimensions);
        memspace_dim.at(0) = dimensions.at(0);
        memspace_dim.at(1) = dimensions.at(1);
        H5::DataSpace memspace( num_dimensions, memspace_dim.data() );
        
        //buffer

        // dataset.read( , H5::PredType::NATIVE_INT, memspace, dataspace );

    }   
}