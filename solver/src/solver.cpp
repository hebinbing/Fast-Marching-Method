#include "fmm/solver.hpp"

namespace fmm
{
    bool solver_t::find_target()
    {   
        bool target_found = 0;

        for(index_t i = 0; i < value_function.dim_size[0]; i++)
        {
            for(index_t j = 0; j < value_function.dim_size[1]; j++)
            {
                if(cost_function(i, j) == -1)
                {
                    target_found = 1;
                    target = point_t{ i, j };
                    value_function(i, j) = 0;
                }
            }
        }

        return target_found;
    }

    function<data_t> solver_t::solve()
    {
        initialize();

        return value_function;
    }

    void solver_t::initialize()
    {
        if(!find_target()){
            std::cerr << "Error: Couldn't find any target" << std::endl;
            return;
        }

        std::vector<point_t> neighbors = get_neighbors(target);

        for(auto i : neighbors)
        {
            narrow_band.insert_or_update(value_function(i[0], i[1]), i);
        }

    }

    // WIP
    std::vector<point_t> solver_t::get_neighbors(point_t c)
    {
        std::vector<point_t> neighbors;

        for(int i = -1, j = 0; i < 2; i += 2)
        {   
            // If i + c[0] < 0 || j + c[0] < 0, it underflows and become positive
            if(i + c[0] < value_function.dim_size[0] &&
               j + c[1] < value_function.dim_size[1])
            {
                point_t p{ i + c[0], j + c[1] };
                neighbors.push_back(p);
            }
        }

        for(int j = -1, i = 0; j < 2; j += 2)
        {   
            // If i + c[0] < 0 || j + c[0] < 0, it underflows and become positive
            if(i + c[0] < value_function.dim_size[0] &&
               j + c[1] < value_function.dim_size[1])
            {
                point_t p{ i + c[0], j + c[1] };
                neighbors.push_back(p);
            }
        }

        return neighbors;
    }

}    // namespace fmm
