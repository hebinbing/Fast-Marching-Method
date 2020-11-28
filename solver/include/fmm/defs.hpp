#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

#define DIM 2

#pragma once

namespace fmm
{
    //! Cost & value function data type
    using data_t = double;

    //! Coordinates index type
    using index_t = std::size_t;

    //! Map coordinates
    using point_t = std::array<index_t, DIM>;

}    // namespace fmm