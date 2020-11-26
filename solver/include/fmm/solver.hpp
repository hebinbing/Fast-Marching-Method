#include <utility>
#include <vector>

#include "defs.hpp"
#include "function.hpp"
#include "heap.hpp"

namespace fmm
{
    template<typename data_t>
    class solver
    {

      public:
        //! Constructior. Moves previously allocated f, allocating it to
        //! solution
        solver(function<data_t> f) : solution(std::move(f)) {
            find_target();
        }

        //! Solver algorithm. Returns solution
        function<data_t> solve();
          
      private:
        function<data_t> solution;

        min_heap<data_t> narrow_band;

        point_t target;

        void find_target();

        data_t eikonal(point_t gridpoint);

        void initialize();

        std::vector<point_t> get_neighbors(point_t c);

        bool neighbor_exists(point_t c)
        {
            if(c[0] < 0 || c[1] < 0 || c[0] > solution.dimension_size() - 1 ||
               c[1] > solution.dimension_size() - 1)
            {
                return 0;
            }

            return 1;
        }
    };

    template<typename data_t>
    function<data_t> solver<data_t>::solve()
    {
        initialize();

        return solution;
    }

    template<typename data_t>
    void solver<data_t>::find_target()
    {
        for(int i = 0; i < solution.dimension_size(); i++)
        {
            for(int j = 0; j < solution.dimension_size(); j++)
            {   
                if(solution(i, j) == -1)
                {
                    target[0] = i;
                    target[1] = j;
                }
            }
        }
    }

    template<typename data_t>
    void solver<data_t>::initialize()
    {
        std::vector<point_t> neighbors =
            get_neighbors(target);

        for(auto i : neighbors)
        {   
            // WIP - insert first neighbors
            // narrow_band.insert_or_update(
                // solution(neighbors.at(i)[0], neighbors.at(i)[1]));
        }
    }

    template<typename data_t>
    std::vector<point_t> solver<data_t>::get_neighbors(point_t c)
    {
        std::vector<point_t> neighbors;

        for(int i = -1, j = 0; i < 2; i += 2)
        {
            point_t p{ i + c[0], j + c[1]};
            if(neighbor_exists(p))
            {
                neighbors.push_back(p);
            }
        }

        for(int j = -1, i = 0; j < 2; j += 2)
        {
            point_t p{ i + c[0], j + c[1]};
            if(neighbor_exists(p))
            {
                neighbors.push_back(p);
            }
        }

        return neighbors;
    }

}    // namespace fmm
