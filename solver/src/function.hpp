/**
 * function class is the data structure of cost and value functions. Cost
 * function is the data structure where the velocity data is. Value function is
 * the data structure where the minimum arrival times data is.
 **/

#include <iostream>
#include <vector>
#include <cstdint>

namespace fmm {

template <typename T> 
class function {
  private:

    //! Function data
    std::vector<T> data;

  public:

    uint64_t dims;
    uint64_t dim_size;
    
    //! Objective index
    uint64_t target_index;
    
    function(uint64_t size) {
      data.resize(size);
    }
    
    function(uint64_t size, uint64_t dimensions, uint64_t dimension_size) : dims(dimensions), dim_size(dimension_size){
      data.resize(size);
    }
    
    T operator()(uint64_t row, uint64_t col) const { return data[col + dim_size*row]; }
    T &operator()(uint64_t row, uint64_t col) { return data[col + dim_size*row]; }


};
}