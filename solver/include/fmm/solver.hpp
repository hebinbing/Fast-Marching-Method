#include <chrono>

#include "defs.hpp"
#include "function.hpp"
#include "heap.hpp"

namespace fmm
{
    class solver_t
    {
      public:
        solver_t(function<data_t> f)
            : cost_function(std::move(f)),
              value_function(f.size(), std::numeric_limits<int>::max()),
              narrow_band(),
              grid_space(2.0 / (value_function.dim_size[0] - 1))
        { std::cout << grid_space << std::endl;}

        //! Solver algorithm. Returns solution
        function<data_t> solve();

      private:
        function<data_t> value_function;
        function<data_t> cost_function;
        min_heap<data_t> narrow_band;

        point_t target;

        data_t grid_space;

        //! Finds target coordinates
        bool find_target();

        //! Initial procedure
        void initialize();

        //! Main loop
        bool iterate();

        //! Get close neighbors
        std::vector<point_t> get_neighbors(point_t c);

        //! Get existing neighbors in specific order
        std::array<data_t, 2*DIM> get_valid_neighbors_values(point_t c);

        //! Calculation of new node value
        data_t update_value(point_t p);
    };

}    // namespace fmm
