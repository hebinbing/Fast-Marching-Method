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
        std::vector<gridpoint_t> data;

        //! Data (square) matrix dimension
        uint64_t dims;
        //! Data (square) matrix size dimension
        uint64_t dim_size;

        //! Objective index in data vector
        uint64_t target_index;

        //! Objective coordinates
        coordinates target_coordinates;

        function() {}
        
        function(uint64_t size, uint64_t dimensions): dims(dimensions), dim_size(pow(size, 1.0 / dimensions))
        {
            data.resize(size);
        }

        function(std::vector<data_t> v, uint64_t dimensions)
            : dims(dimensions), dim_size(pow(v.size(), 1.0 / dimensions))
        {   
            for(int row = 0, col = 0, i = 0; i < v.size(); i++, col++){

                if(col==dim_size){
                    col=0;
                    row++;
                }

                data.push_back(gridpoint_t { v.at(i), std::pair<int,int> {row, col} });
            }

            find_target_index();
        }

        gridpoint_t operator()(int row, int col) const
        {
            return get_point(row, col);
        }

        gridpoint_t& operator()(int row, int col)
        {
            return get_point(row, col);
        }

        void print();

        size_t size() { return data.size(); }

      private:

        gridpoint_t get_point(int row, int col) const{
            return data.at(col + dim_size * row);
        }

        gridpoint_t& get_point(int row, int col){
            return data.at(col + dim_size * row);
        }

        void find_target_index();

    };

    template<typename data_t>
    void function<data_t>::find_target_index()
    {
        for(int i = 0; i < dim_size; i++)
        {
            for(int j = 0; j < dim_size; j++)
            {           
                if(get_point(i,j).value == -1){
                    target_index = j + dim_size * i;
                    target_coordinates = {i, j};
                }
            }
        }
    }

    template<typename data_t>
    void function<data_t>::print()
    {
        std::cout << "Function Data : Total size = " << size()
                << " | Number of Dimensions = " << dims
                << " | Dimension size = " << dim_size << std::endl;

        for(int i = 0; i < dim_size; i++)
        {
            for(int j = 0; j < dim_size; j++)
            {   
            std::cout << get_point(i,j).value << "|" << get_point(i,j).map_index.first << "," << get_point(i,j).map_index.second << std::endl;
            }
        }
    }

}    // namespace fmm