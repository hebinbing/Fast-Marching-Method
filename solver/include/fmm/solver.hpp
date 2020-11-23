#include "defs.hpp"
#include "function.hpp"
#include "heap.hpp"
#include <utility>

namespace fmm
{   

    template<typename data_t>
    class solver
    {
      private:

        function<data_t> solution;

        data_t eikonal(gridpoint_t gridpoint);

        void initialize();

        std::vector<coordinates> get_neighbors();

      public:

        //! Constructior. Moves previously allocated f, allocating it to solution
        solver(function<data_t> f) : solution(std::move(f)) {}

        //! Solver algorithm. Returns solution
        function<data_t> solve();
    };

    template<typename data_t>
    function<data_t> solver<data_t>::solve(){
    
      return solution;
    }

    template<typename data_t>
    void solver<data_t>::initialize(){
      
      solution(solution.target_coordinates.first, solution.target_coordinates.second).status = ALIVE;

      // WIP
    }

    template<typename data_t>
    std::vector<coordinates> solver<data_t>::get_neighbors(){
      
      // WIP
    }

}    // namespace fmm


