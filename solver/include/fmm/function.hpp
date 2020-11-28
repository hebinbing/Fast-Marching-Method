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

      private:
        //! Function data
        std::vector<T> data;

        //! Data (square) matrix size dimension
        // size_t dim_size;
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

}    // namespace fmm