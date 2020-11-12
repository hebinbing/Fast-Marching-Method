#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

namespace fmm {

template <typename T> class min_heap {
  private:
    struct node{
      T value;
      uint64_t index;
    };
    
    std::vector<node> data;
    
    //! Updates the value of an existing node
    void update();

    uint64_t parent_index(uint64_t index) { return floor((index - 1) / 2); }

    uint64_t left_child_index(uint64_t index) { return floor((index + 1) / 2); }

    uint64_t right_child_index(uint64_t index) { return floor((index + 2) / 2); }

    uint64_t min_child_index(uint64_t index) { return std::min(right_child_index(index), left_child_index(index)); }
    
    uint64_t parent(uint64_t index) { return data.at(parent_index(index)); }

    uint64_t left_child(uint64_t index) { return data.at(left_child_index(index)); }

    uint64_t right_child(uint64_t index) { return data.at(right_child_index(index)); }

    uint64_t min_child(uint64_t index) { return data.at(min_child_index(index)); }

  public:
    //! Returns the min node value
    T const min_node() {
        if (data.size() == 0)
            std::cerr << "Heap is empty." << std::endl;

        return data.value[0];
    }

    //! Inserts a node
    void insert(T new_node_value, uint64_t new_node_index);

    //! Removes the node with smallest value
    void pop();

    //! Bubbles down a node
    void down_heap(uint64_t index);

    //! Bubbles up a node
    void up_heap(uint64_t index);
};
}

template <typename T> 
void fmm::min_heap<T>::update() {}

template <typename T> 
void fmm::min_heap<T>::insert(T new_node_value, uint64_t new_node_index) {
    node new_node;

    new_node.value = new_node_value;
    new_node.index = new_node_index;

    data.push_back(new_node);

    up_heap(data.size() - 1);
}

template <typename T> 
void fmm::min_heap<T>::pop() {
  if (data.size() == 0)
        std::cerr << "Heap is empty." << std::endl;
}

template <typename T> 
void fmm::min_heap<T>::down_heap(uint64_t index) {

  //for cycle that swaps parent w/ children when node.value is smaller
      //compare node.value

  for(auto child = min_child(index); data[index].value != data.end().value; index = min_child_index(index)){
    
    if(data.at(index).value > child.value)
      std::swap(data.at(index), data.at(child.index));
    
  }

  //what to return?
}

template <typename T> 
void fmm::min_heap<T>::up_heap(uint64_t index) {

  //for cycle that swaps children w/ parent when node.value is smaller
      //compare node.value

  for(auto par = parent(index); data[index].value != data.begin().value; index = par.index){
    
    if(data.at(index).value < par.value)
      std::swap(data.at(index), data.at(par.index));
    
  }

  //what to return?
}
