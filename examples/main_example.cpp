#include <iostream>

#include "hdf5_parser.hpp"
#include <chrono>
#include <fstream>

int main()
{
    char const* file_name_r = "../data/velocity_data.h5";
    char const* dataset_name_r = "velocities";

    char const* file_name_w = "../data/value_function.h5";
    char const* dataset_name_w = "value_function";

    fmm::function<fmm::data_t> cost_function =
        io::read(file_name_r, dataset_name_r);

    fmm::solver_t s(cost_function);
    auto start = std::chrono::high_resolution_clock::now();
    fmm::function<fmm::data_t> value_function = s.solve();
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::ofstream ofs("../data/last_measured_time.txt", std::ofstream::trunc);
    ofs << duration.count();
    ofs.close();

    io::write(file_name_w, dataset_name_w, value_function);

    // Debug
    // cost_function.print_row_major();
    // value_function.print_row_major();
}