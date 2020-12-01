#include <stdio.h>

#include "defs.hpp"

namespace fmm
{
    template<typename T>
    class function
    {
      public:
        //! Empty constructor
        function() {}

        //! Square domain constructor
        function(size_t max_size)
        {
            for(int i = 0; i < DIM; i++)
                dim_size[i] = pow(max_size, (1.0 / DIM));

            data.reserve(max_size);
        }

        //! Square domain constructor w/ initial value
        function(size_t max_size, T initial_value)
        {
            for(int i = 0; i < DIM; i++)
                dim_size[i] = pow(max_size, (1.0 / DIM));

            data = std::vector<T>(max_size, initial_value);
        }

        //! Square domain constructor w/ initial values
        function(std::vector<double> v)
        {
            for(int i = 0; i < DIM; i++)
                dim_size[i] = pow(v.size(), (1.0 / DIM));
            data = v;
        }

        //! Value at given coordinates, through row-major-order index
        T operator()(int row, int col) const
        {
            return data.at(col + dim_size[0] * row);
        }

        //! Reference to value at given coordinates, through row-major-order
        //! index
        T& operator()(int row, int col)
        {
            return data.at(col + dim_size[0] * row);
        }

        T* get_data() { return data.data(); }

        size_t size() { return data.size(); }

        //! Dimensions size
        std::array<index_t, DIM> dim_size;

        //! Delete this temp function when finished
        void print();

        //! Delete this temp function when finished
        void print_row_major();

      private:
        //! Function data
        std::vector<T> data;
    };

    template<typename data_t>
    void function<data_t>::print()
    {
        std::cout << "Function Data : Total size = " << size()
                  << " | Number of Dimensions = " << DIM
                  << " | Dimensions = " << dim_size[0] << "x" << dim_size[1]
                  << std::endl;

        for(int i = 0; i < size(); i++)
        {
            std::cout << data.at(i) << std::endl;
        }
    }

    template<typename data_t>
    void function<data_t>::print_row_major()
    {
        std::cout << "Function Data : Total size = " << size()
                  << " | Number of Dimensions = " << DIM
                  << " | Dimensions = " << dim_size[0] << "x" << dim_size[1]
                  << std::endl;

        for(int i = 0; i < dim_size[0]; i++)
        {
            for(int j = 0; j < dim_size[1]; j++)
            {
                printf("%.3f ", data.at(j + dim_size[0] * i));
            }

            std::cout << std::endl;
        }
    }

}    // namespace fmm