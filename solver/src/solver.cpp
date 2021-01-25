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
                    targets.push_back( point_t{i, j}) ;
                    value_function(i, j) = 0;
                }
            }
        }
        
        return !targets.empty();
    }

    function<data_t> solver_t::solve()
    {
        int cycle = 0;

        initialize();

        while(iterate())
        {
            std::cout << "Iteration nº: " << cycle++ << std::endl;
        }

        return value_function;
    }

    void solver_t::initialize()
    {
        if(!find_targets())
        {
            std::cerr << "Error: Couldn't find any target" << std::endl;
            return;
        }

        for( auto t : targets)
            narrow_band.insert_or_update(value_function(t[0], t[1]),
                                     t);
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
        
        for (auto t : targets)
            if(p[0] == t[0] && p[1] == t[1]){
                return 1;
}
        return 0;
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

        // std::cout << "Trial " << trial[0] << "|" << trial[1] << " - value : " << value_function(trial[0], trial[1]) << "\n" << std::endl;

        for(auto i : neighbors)
        {   
            // std::cout << "Trail neighbor: " << i[0] << "|" << i[1] << " - value : " << value_function(i[0], i[1]) << std::endl;
            // ALIVE points do not get updated
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

            if(is_boundary(i) && !is_target(i))
            {   

                auto old_val = value_function(i[0], i[1]);
                auto new_val = update_boundary_point_value(i);

                if(new_val > old_val){
                    // std::cout << "Second update: " << new_val << std::endl;
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
            // std::cout << "Boundary corner : " << p[0] << "|" << p[1] << std::endl;
            
            std::vector<data_t> valid_neighs;
            for(auto i : neighbors){
                if(i < std::numeric_limits<int>::max()){
                    valid_neighs.push_back(i);
                }
            }

            val = std::max(*valid_neighs.begin(), *valid_neighs.end());

        } else{
            // std::cout << "Boundary point : " << p[0] << "|" << p[1] << std::endl;
            if(p[0] == 0){
                if(neighbors[1] < std::numeric_limits<int>::max())
                    val = neighbors[1];
            } else if(p[0] == value_function.dim_size[0] - 1){
                if(neighbors[0] < std::numeric_limits<int>::max())
                    val = neighbors[0];
            } else if(p[1] == 0){
                if(neighbors[3] < std::numeric_limits<int>::max())
                    val = neighbors[3];
            } else if(p[1] == value_function.dim_size[1] - 1){
                if(neighbors[2] < std::numeric_limits<int>::max())
                    val = neighbors[2];
            } 
        }

        // std::cout << "New boundary value : " << std::max(val, value_function(p[0], p[1])) << std::endl;

        return std::max(val, value_function(p[0], p[1]));
    }

}    // namespace fmm
