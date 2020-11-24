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
      private:
        function<data_t> solution;

        min_heap<data_t> narrow_band;

        data_t eikonal(gridpoint_t gridpoint);

        void initialize();

        std::vector<coordinates> get_neighbors(coordinates c);

        bool neighbor_exists(coordinates c)
        {
            if(c.first < 0 || c.second < 0 || c.first > solution.dim_size - 1 ||
               c.second > solution.dim_size - 1)
            {
                return 0;
            }

            return 1;
        }

      public:
        //! Constructior. Moves previously allocated f, allocating it to
        //! solution
        solver(function<data_t> f) : solution(std::move(f)) {}

        //! Solver algorithm. Returns solution
        function<data_t> solve();
    };

    template<typename data_t>
    function<data_t> solver<data_t>::solve()
    {
        initialize();

        return solution;
    }

    template<typename data_t>
    void solver<data_t>::initialize()
    {
        std::vector<coordinates> neighbors =
            get_neighbors(solution.target_coordinates);

        for(int i = 0; i < neighbors.size(); i++)
        {
            narrow_band.insert_or_update(
                solution(neighbors.at(i).first, neighbors.at(i).second));
        };
    }

    template<typename data_t>
    std::vector<coordinates> solver<data_t>::get_neighbors(coordinates c)
    {
        std::vector<coordinates> neighbors;

        for(int i = -1, j = 0; i < 2; i += 2)
        {
            coordinates temp_c{ i + c.first, j + c.second };
            if(neighbor_exists(temp_c))
            {
                neighbors.push_back(temp_c);
            }
        }

        for(int j = -1, i = 0; j < 2; j += 2)
        {
            coordinates temp_c{ i + c.first, j + c.second };
            if(neighbor_exists(temp_c))
            {
                neighbors.push_back(temp_c);
            }
        }

        return neighbors;
    }

}    // namespace fmm
