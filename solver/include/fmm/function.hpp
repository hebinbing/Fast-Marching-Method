/**
 * function class is the data structure of cost and value functions. Cost
 * function is the data structure where the velocity data is. Value function is
 * the data structure where the minimum arrival times data is.
 **/

#include <cstdint>
#include <iostream>
#include <vector>
#include <cmath>

#include "defs.hpp"

namespace fmm
{
    template<typename data_t>
    class function
    {
      public:
        //! Function data
        std::vector<data_t> data;

        //! Data (square) matrix dimension
        uint64_t dims;
        //! Data (square) matrix size dimension
        uint64_t dim_size;

        //! Objective index
        uint64_t target_index;

        function(uint64_t size, uint64_t dimensions): dims(dimensions), dim_size(pow(size, 1.0 / dimensions))
        {
            data.resize(size);
        }

        function(std::vector<data_t> v, uint64_t dimensions)
            : dims(dimensions), dim_size(pow(v.size(), 1.0 / dimensions))
        {
            data = v;
            find_target_index();
        }

        void find_target_index();

        void print();

        data_t operator()(uint64_t row, uint64_t col) const
        {
            return data[col + dim_size * row];
        }
        data_t& operator()(uint64_t row, uint64_t col)
        {
            return data[col + dim_size * row];
        }

        size_t size() { return data.size(); }
    };
}    // namespace fmm

template<typename data_t>
void fmm::function<data_t>::find_target_index()
{
    for(uint64_t i = 0; i < dim_size; i++)
    {
        for(uint64_t j = 0; j < dim_size; j++)
        {
            if(data.at(i + dim_size * j) == -1)
                target_index = j + dim_size * i;
        }
    }
}

template<typename data_t>
void fmm::function<data_t>::print()
{
    std::cout << "Function Data : Total size = " << size()
              << " | Number of Dimensions = " << dims
              << " | Dimension size = " << dim_size << std::endl;

    for(uint64_t i = 0; i < dim_size; i++)
    {
        for(uint64_t j = 0; j < dim_size; j++)
        {
            std::cout << "(" << i << "," << j
                      << ") = " << data.at(i + dim_size * j) << std::endl;
        }
    }
}