#include <iostream>

#include "hdf5_parser.hpp"

int main()
{
    // Temporary strings - in the future, will pass them as main arguments (to
    // be inserted by python script)
    char const* file_name_r = "../data/velocity_data.h5";
    char const* dataset_name_r = "velocities";

    char const* file_name_w = "../data/value_function.h5";
    char const* dataset_name_w = "value_function";

    fmm::function<fmm::data_t> cost_function =
        io::read(file_name_r, dataset_name_r);

    fmm::solver<fmm::data_t> s;

    fmm::function<fmm::data_t> value_function = s.solve(cost_function);

    io::write(file_name_w, dataset_name_w, value_function);

    value_function = io::read(file_name_w, "value_function");

    // Debug
    value_function.print();
}