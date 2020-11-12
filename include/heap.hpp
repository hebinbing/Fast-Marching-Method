#include <iostream>
#include <vector>
#include <cmath>

namespace fmm {

template <typename T>
class min_heap {
  private:
    
    std::vector<T> data;

    //! Update the value of an existing node
    void update();

    int parent_index(int index) { return floor((index-1)/2); }

    int left_child_index(int index) { return floor((index+1)/2); }

    int right_child_index(int index) { return floor((index+2)/2); }

    int parent(int index) { return data[parent_index(index)]; }

    int left_child(int index) { return data[left_child_index(index)]; }

    int right_child(int index) { return data[right_child_index(index)]; }

    void swap (int index_one, int index_two) {
      T temp = data[index_one];
      data[index_one] = data[index_two];
      data[index_two] = temp;
    }

  public:
    
    //! Return the node with smallest value
    T const min_node() {
      if (data.size() == 0)
          std::cerr << "Heap is empty." << std::endl;
      
      return data[0];      
    }

    //! Insert a node
    void insert();
    
    //! Remove the node with smallest value
    void pop();
    
    //! Bubble down a node
    void down_heap();

    //! Bubble up a node
    void up_heap();

};
}

template <typename T>
void update(){

}

template <typename T>
void insert(){

}

template <typename T>
void pop(){

}

template <typename T>
void down_heap(){

}

template <typename T>
void up_heap(){

}
