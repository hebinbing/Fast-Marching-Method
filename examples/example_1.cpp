#include <iostream>

#include "hdf5_parser.hpp"

int main(){
    
    // Temporary strings - in the future, will pass them as main arguments (to be inserted by python script)
    char const *file_name_r = "../data/velocity_data.h5";
    char const *dataset_name_r = "velocities";
    
    fmm::function<fmm::data_t> cost_function = io::read(file_name_r, dataset_name_r);


    //Value function test
    char const *file_name_w = "../data/value_function.h5";
    char const *dataset_name_w = "value_function";

    io::write(file_name_w, dataset_name_w, cost_function);

    fmm::function<fmm::data_t> value_function = io::read(file_name_w, "value_function");

}