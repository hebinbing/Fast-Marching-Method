#include <cmath>
#include <iostream>
#include <vector>

namespace fmm {

template <typename T> class min_heap {
  private:
    //! Updates the value of an existing node
    void update();

    uint64_t parent_index(uint64_t index) { return floor((index - 1) / 2); }

    uint64_t left_child_index(uint64_t index) { return floor((index + 1) / 2); }

    uint64_t right_child_index(uint64_t index) { return floor((index + 2) / 2); }

    uint64_t parent(uint64_t index) { return data[parent_index(index)]; }

    uint64_t left_child(uint64_t index) { return data[left_child_index(index)]; }

    uint64_t right_child(uint64_t index) { return data[right_child_index(index)]; }

    struct node{
      T value;
      uint64_t index;
    }
  
    std::vector<node> data;

  public:
    //! Returns the min node value
    T const min_node() {
        if (data.size() == 0)
            std::cerr << "Heap is empty." << std::endl;

        return data.value[0];
    }

    //! Inserts a node
    void insert();

    //! Removes the node with smallest value
    void pop();

    //! Bubbles down a node
    void down_heap();

    //! Bubbles up a node
    void up_heap();
};
}

template <typename T> 
void update() {}

template <typename T> 
void insert(T new_node_value, uint64_t new_node_index) {
    node new_node;

    new_node.value = new_node_value;
    new_node.index = new_node_index;

    data.push_back(new_node);

    up_heap(data.size() - 1);
}

template <typename T> 
void pop() {
  if (data.size() == 0)
        std::cerr << "Heap is empty." << std::endl;
}

template <typename T> 
void down_heap() {

  //find the node with given index

  //for cycle that swaps parent w/ children when node.value is smaller
      //compare node.value

  //what to return?
}

template <typename T> 
void up_heap(uint64_t index) {
  
  //find the node with given index

  //for cycle that swaps children w/ parent when node.value is smaller
      //compare node.value

  //what to return?
}
