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
    
    function(uint64_t size, uint64_t dimensions) : dims(dimensions), dim_size(pow(size, 1.0/dimensions)){
      data.resize(size);
    }

    function(std::vector<float> v, uint64_t dimensions) : dims(dimensions), dim_size(pow(v.size(), 1.0/dimensions)) {
      data = v;
    }
    
    T operator()(uint64_t row, uint64_t col) const { return data[col + dim_size*row]; }
    T &operator()(uint64_t row, uint64_t col) { return data[col + dim_size*row]; }

    size_t size() { return data.size(); }

};
}