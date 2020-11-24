#include <utility>

#pragma once

namespace fmm
{
    //! Cost & value function data type
    using data_t = double;

    //! Map coordinates pair (x,y)
    using coordinates = std::pair<int, int>;

    //! Grid point data structure
    struct gridpoint_t
    {
        data_t value;
        coordinates map_index;
    };

}    // namespace fmm