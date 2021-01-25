#include "fmm/solver.hpp"
#include <algorithm>

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
        int cycle = 0;

        initialize();

        while(iterate())
        {
            std::cout << "Iterat1ion nº: " << cycle++ << std::endl;
            // narrow_band.print();
        }

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

    bool solver_t::is_boundary(point_t p){

        if(p[0] == value_function.dim_size[0] - 1 || p[0] == 0 || p[1] == value_function.dim_size[1] - 1 || p[1] == 0)
            return 1;
        
        return 0;
    }

    bool solver_t::is_boundary_corner(point_t p){
        
        if(p[0] == 0 || p[0] == value_function.dim_size[0] - 1)
            if(p[1] == 0 || p[1] == value_function.dim_size[1] - 1)
                return 1;

        return 0;    
    }

    bool solver_t::is_target(point_t p){

        if(p[0] == target[0] && p[1] == target[1])
            return 1;

        return 0;
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

            if(is_boundary(i) && !is_target(i))
            {
                auto old_val = value_function(i[0], i[1]);
                auto new_val = update_boundary_point_value(i);

                if(new_val > old_val){
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

    data_t solver_t::update_boundary_point_value(point_t p){
        
        auto neighbors = get_valid_neighbors_values(p);
        data_t val = 0;
        
        if(is_boundary_corner(p)){
            std::cout << "Found a corner : " << p[0] << "|" << p[1] << std::endl;
            
            std::vector<data_t> valid_neighs;
            for(auto i : neighbors){
                if(i < std::numeric_limits<int>::max()){
                    valid_neighs.push_back(i);
                }
            }

            val = std::max(*valid_neighs.begin(), *valid_neighs.end());
            std::cout << "max val boundary corner: " << val << std::endl;

        } else{
            if(p[0] == 0){
                val = neighbors[1];
            } else if(p[0] == value_function.dim_size[0] - 1){
                val = neighbors[0];
            } else if(p[1] == 0){
                val = neighbors[3];
            } else if(p[1] == value_function.dim_size[1] - 1){
                val = neighbors[2];
            } 
        }

        return std::max(val, value_function(p[0], p[1]));
    }

}    // namespace fmm
