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
            // narrow_band.print();
            std::cout << "Iteration nº: " << cycle++ << std::endl;
        }

        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = stop - start;

        std::cout << "Elapsed time = " << duration.count() << " seconds"
                  << std::endl;

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

    std::array<data_t, 2 * DIM> solver_t::get_valid_neighbors_values(point_t c)
    {
        std::array<data_t, 2 * DIM> neighbors_vals;
        neighbors_vals.fill(std::numeric_limits<data_t>::max());

        int k = 0;

        for(int i = -1, j = 0; i < 2; i += 2, k++)
        {
            // If i + c[0] < 0 || j + c[0] < 0, it underflows and become
            // positive
            if(i + c[0] < value_function.dim_size[0] &&
               j + c[1] < value_function.dim_size[1])
            {
                neighbors_vals.at(k) = value_function(i + c[0], j + c[1]);
            }

            // k++;
        }

        for(int j = -1, i = 0; j < 2; j += 2, k++)
        {
            // If i + c[0] < 0 || j + c[0] < 0, it underflows and become
            // positive
            if(i + c[0] < value_function.dim_size[0] &&
               j + c[1] < value_function.dim_size[1])
            {
                neighbors_vals.at(k) = value_function(i + c[0], j + c[1]);
            }

            // k++;
        }

        return neighbors_vals;
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

        // std::cout << "Trial = "
        //           << "(" << trial[0] << "," << trial[1] << ") - ("
        //           << value_function(trial[0], trial[1]) << ")" << std::endl;

        for(auto i : neighbors)
        {
            // ALIVE points do not get updated
            if(value_function(i[0], i[1]) >= value_function(trial[0], trial[1]))
            {
                auto old_val = value_function(i[0], i[1]);
                auto new_val = update_value(i);
                // std::cout << "Neighbor = "
                //           << "(" << i[0] << "," << i[1]
                //           << ") | old_val = " << old_val
                //           << " | new_val = " << new_val << std::endl;

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
        auto const cost = grid_space * 1 / cost_function(p[0], p[1]);
        auto const cost_square = cost * cost;

        auto min_candidates = get_valid_neighbors_values(p);

        auto a = std::min(min_candidates[0], min_candidates[1]);
        auto b = std::min(min_candidates[2], min_candidates[3]);

        return cost > std::abs(a - b)
                   ? 0.5 * (a + b + sqrt(2 * cost_square - (a - b) * (a - b)))
                   : cost + std::min(a, b);

    }

}    // namespace fmm
