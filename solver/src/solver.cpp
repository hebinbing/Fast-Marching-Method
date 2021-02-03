#include "fmm/solver.hpp"
#include <algorithm>

namespace fmm
{
    bool solver_t::find_targets()
    {
        for(index_t i = 0; i < value_function.dim_size[0]; i++)
        {
            for(index_t j = 0; j < value_function.dim_size[1]; j++)
            {
                if(cost_function(i, j) == -1)
                {
                    targets.push_back(point_t{ i, j });
                    value_function(i, j) = 0;
                }
            }
        }
        return targets.empty();
    }

    bool solver_t::find_obstacles(){
        for(index_t i = 0; i < value_function.dim_size[0]; i++)
        {
            for(index_t j = 0; j < value_function.dim_size[1]; j++)
            {
                if(cost_function(i, j) < 0 && cost_function(i, j) != -1)
                {
                    obstacles.push_back(point_t{ i, j });
                    cost_function(i, j) = -cost_function(i, j);
                    value_function(i, j) = 1 / cost_function(i, j);
                }
            }
        }
        return obstacles.empty();
    }

    function<data_t> solver_t::solve()
    {
        int cycle = 0;

        initialize();

        while(iterate())
        {
            cycle++;
#ifdef NDEBUG
            std::cerr << "Iteration nº: " << cycle++ << std::endl;
#endif
        }
        std::cerr << "Nº of iterations : " << cycle << std::endl;

        return value_function;
    }

    void solver_t::initialize()
    {
        if(find_targets())
        {
            std::cerr << "Error: Couldn't find any target." << std::endl;
            return;
        }

        if(find_obstacles())
        {
            std::cerr << "No obstacles found." << std::endl;
        }

        for(auto t : targets)
            narrow_band.insert_or_update(value_function(t[0], t[1]), t);

        for(auto obs : obstacles)
            narrow_band.insert_or_update(value_function(obs[0], obs[1]), obs);
    }

    std::array<data_t, 2 * DIM> solver_t::get_valid_neighbors_values(point_t p)
    {
        std::array<data_t, 2 * DIM> neighbors_vals;
        neighbors_vals.fill(std::numeric_limits<int>::max());

        int k = 0;

        for(int i = -1, j = 0; i < 2; i += 2, k++)
        {
            // If i + p[0] < 0 || j + p[0] < 0, it underflows and become
            // positive
            if(i + p[0] < value_function.dim_size[0] &&
               j + p[1] < value_function.dim_size[1])
            {
                neighbors_vals.at(k) = value_function(i + p[0], j + p[1]);
            }
        }

        for(int j = -1, i = 0; j < 2; j += 2, k++)
        {
            // If i + p[0] < 0 || j + p[0] < 0, it underflows and become
            // positive
            if(i + p[0] < value_function.dim_size[0] &&
               j + p[1] < value_function.dim_size[1])
            {
                neighbors_vals.at(k) = value_function(i + p[0], j + p[1]);
            }
        }

        return neighbors_vals;
    }

    std::vector<point_t> solver_t::get_neighbors(point_t p)
    {
        std::vector<point_t> neighbors;

        for(int i = -1, j = 0; i < 2; i += 2)
        {
            // If i + p[0] < 0 || j + p[0] < 0, it underflows and become
            // positive
            if(i + p[0] < value_function.dim_size[0] &&
               j + p[1] < value_function.dim_size[1])
            {
                neighbors.push_back(point_t{ i + p[0], j + p[1] });
            }
        }

        for(int j = -1, i = 0; j < 2; j += 2)
        {
            // If i + p[0] < 0 || j + p[0] < 0, it underflows and become
            // positive
            if(i + p[0] < value_function.dim_size[0] &&
               j + p[1] < value_function.dim_size[1])
            {
                neighbors.push_back(point_t{ i + p[0], j + p[1] });
            }
        }

        return neighbors;
    }

    bool solver_t::iterate()
    {
        // std::cout << std::endl;
        // narrow_band.print();
        if(narrow_band.size() == 0)
        {
            return 0;
        }

        auto trial = narrow_band.min_node_coordinates();
        narrow_band.pop();

        std::vector<point_t> neighbors = get_neighbors(trial);

        // std::cout << "Trial " << trial[0] << "|" << trial[1] << " - value : "
        // << value_function(trial[0], trial[1]) << "\n" << std::endl;

        for(auto i : neighbors)
        {
            // std::cout << "Trail neighbor: " << i[0] << "|" << i[1] << " -
            // value : " << value_function(i[0], i[1]) << std::endl; ALIVE
            // points do not get updated
            if(value_function(i[0], i[1]) >= value_function(trial[0], trial[1]))
            {
                auto old_val = value_function(i[0], i[1]);
                auto new_val = update_value(i);

                if(new_val < old_val)
                {
                    // std::cout << "First update: " << new_val << std::endl;
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
