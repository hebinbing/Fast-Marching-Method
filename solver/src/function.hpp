/**
 * function class is the data structure of cost and value functions. Cost
 * function is the data structure where the velocity data is. Value function is
 * the data structure where the minimum arrival times data is.
 **/

#include <iostream>
#include <vector>
#include <cstdint>

namespace fmm {

template <typename T, uint64_t dim> 
class function {
  private:

    //! Function data
    std::vector<T> data;

  public:

    function(uint64_t size) {
      data.resize(size);
    }
    
    function(uint64_t size, uint64_t tar_idx) : target_index(tar_idx) {
      data.resize(size);
    }

    //! Objective index
    uint64_t target_index;
    
    T operator()(uint64_t row, uint64_t col) const { return data[col + dim*row]; }
    T &operator()(uint64_t row, uint64_t col) { return data[col + dim*row]; }


};
}