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
        auto start = std::chrono::high_resolution_clock::now(); 
        int cycle = 0;

        initialize();

        while(iterate())
        {
            std::cout << "Iteration nº: " << cycle++ << std::endl;
        }

        auto stop = std::chrono::high_resolution_clock::now(); 
        std::chrono::duration<double> duration = stop - start;

        std::cout << "Elapsed time = " << duration.count() << " seconds" << std::endl;

        return value_function;
    }

    void solver_t::initialize()
    {
        if(!find_target())
        {
            std::cerr << "Error: Couldn't find any target" << std::endl;
            return;
        }

        narrow_band.insert_or_update(value_function(target[0], target[1]),
                                     target);
    }

    std::vector<point_t> solver_t::get_neighbors(point_t c)
    {
        std::vector<point_t> neighbors;

        for(int i = -1, j = 0; i < 2; i += 2)
        {
            // If i + c[0] < 0 || j + c[0] < 0, it underflows and become
            // positive
            if(i + c[0] < value_function.dim_size[0] &&
               j + c[1] < value_function.dim_size[1])
            {
                point_t p{ i + c[0], j + c[1] };
                neighbors.push_back(p);
            }
        }

        for(int j = -1, i = 0; j < 2; j += 2)
        {
            // If i + c[0] < 0 || j + c[0] < 0, it underflows and become
            // positive
            if(i + c[0] < value_function.dim_size[0] &&
               j + c[1] < value_function.dim_size[1])
            {
                point_t p{ i + c[0], j + c[1] };
                neighbors.push_back(p);
            }
        }

        return neighbors;
    }

    bool solver_t::iterate()
    {
        if(narrow_band.size() == 0)
        {
            return 0;
        }

        auto trial = narrow_band.min_node_coordinates();
        narrow_band.pop();

        std::vector<point_t> neighbors = get_neighbors(trial);

        for(auto i : neighbors)
        {
            // ALIVE points do not get updated
            if(value_function(i[0], i[1]) >= value_function(trial[0], trial[1]))
            {
                auto old_val = value_function(i[0], i[1]);
                auto new_val = update_value(i);

                if(new_val < old_val)
                {   
                    value_function(i[0], i[1]) = new_val;
                    narrow_band.insert_or_update(new_val, i);
                }
            }
        }

        return 1;
    }

    data_t solver_t::update_value(point_t p)
    {
        auto val = data_t{ 0.0 };
        auto cost_square =
            cost_function(p[0], p[1]) * cost_function(p[0], p[1]);

        std::array<data_t, 2 * DIM> min_candidates;
        min_candidates.fill(std::numeric_limits<int>::max());

        for(auto i : get_neighbors(p))
        {
            if(i[0] == p[0])
            {
                if(i[1] < p[1])
                {
                    min_candidates[2] = value_function(i[0], i[1]);
                }
                else
                {
                    min_candidates[3] = value_function(i[0], i[1]);
                }
            }
            else if(i[1] == p[1])
            {
                if(i[0] < p[0])
                {
                    min_candidates[0] = value_function(i[0], i[1]);
                }
                else
                {
                    min_candidates[1] = value_function(i[0], i[1]);
                }
            }
        }

        auto a = std::min(min_candidates[0], min_candidates[1]);
        auto b = std::min(min_candidates[2], min_candidates[3]);

        cost_square > std::abs(a - b)
            ? val = 0.5 * (a + b + sqrt(2 * cost_square - (a - b) * (a - b)))
            : val = cost_square + std::min(a, b);

        return val;
    }

}    // namespace fmm
