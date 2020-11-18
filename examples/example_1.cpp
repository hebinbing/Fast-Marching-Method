#include <iostream>

#include "hdf5_parser.hpp"

int main(){
    
    // Temporary strings - in the future, will pass them as main arguments (to be inserted by python script)
    char const *file_name = "../data/velocity_data.h5";
    char const *dataset_name = "velocities";
    
    io::read(file_name, dataset_name);
}