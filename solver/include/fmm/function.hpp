/**
 * function class is the data structure of cost and value functions. Cost
 * function is the data structure where the velocity data is. Value function is
 * the data structure where the minimum arrival times data is.
 **/

#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

#include "defs.hpp"

namespace fmm
{
    template<typename data_t>
    class function
    {
      public:
        // //! Objective index in data vector
        // uint64_t target_index;

        // //! Objective coordinates
        // coordinates target_coordinates;

        function() {}

        function(int npts)
        {   
            dim_size = pow(npts, (1.0/DIM));
            
            data.resize(npts);
        }

        function(std::vector<data_t> v)
        {
            dim_size = pow(v.size(), 1.0/DIM);
            data = v;
        }

        data_t operator()(int row, int col) const
        {
            return data.at(col + dim_size * row);
        }

        data_t& operator()(int row, int col)
        {
            return data.at(col + dim_size * row);
        }

        data_t* get_data()
        {
            return data.data();
        }

        //! Delete this temp function when finished
        void print();

        size_t size() { return data.size(); }

        size_t dimension_size() { return dim_size; }

      private:
        //! Function data
        std::vector<data_t> data;

        //! Data (square) matrix size dimension
        size_t dim_size;

    };

    template<typename data_t>
    void function<data_t>::print()
    {
        std::cout << "Function Data : Total size = " << size()
                  << " | Number of Dimensions = " << DIM
                  << " | Dimension size = " << dim_size << std::endl;

        for(int i = 0; i < size(); i++)
        {
                std::cout << data.at(i) << std::endl;
        }
    }

}    // namespace fmm