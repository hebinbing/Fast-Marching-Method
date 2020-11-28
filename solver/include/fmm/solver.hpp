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
              narrow_band() 
        {}

        //! Solver algorithm. Returns solution
        function<data_t> solve();

      private:
        function<data_t> value_function;
        function<data_t> cost_function;
        min_heap<data_t> narrow_band;
        
        point_t target;

        bool find_target();

        void initialize();

        //WIP
        std::vector<point_t> get_neighbors(point_t c);
        
    };

}    // namespace fmm
