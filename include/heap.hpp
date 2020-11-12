#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

namespace fmm {

template <typename T> class min_heap {
  private:
    struct node {
        T value;
        uint64_t index;
    };

    std::vector<node> data;

    //! Updates the value of an existing node
    void update();

    uint64_t parent(uint64_t index) { return floor((index - 1) / 2); }

    uint64_t min_child(uint64_t index);

  public:
    //! Min_heap constructor
    min_heap(uint64_t max_size);

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

    //! Returns data vector size
    uint64_t size() { return data.size(); }

    //! Debug temp pruint64_t function
    void print();
};
}

template <typename T> fmm::min_heap<T>::min_heap(uint64_t max_size) {
    data.reserve(max_size);
}

template <typename T> uint64_t fmm::min_heap<T>::min_child(uint64_t index) {
    if (data.size() <= 2 * index + 1) {
        return index;

    } else if (data.size() - 1 == 2 * index + 1) {
        return 2 * index + 1;

    } else {
    return (data.at(2*index + 1).value <= data.at(2*index +2).value) ? (2*index + 1) : (2*index + 2);
    }
}

template <typename T> void fmm::min_heap<T>::update() {}

template <typename T>
void fmm::min_heap<T>::insert(T new_node_value, uint64_t new_node_index) {
    node new_node;

    new_node.value = new_node_value;
    new_node.index = new_node_index;

    data.push_back(new_node);

    up_heap(data.size() - 1);
}

template <typename T> void fmm::min_heap<T>::pop() {
    if (data.size() == 0) {
        std::cerr << "Heap is empty." << std::endl;
        return;
    }

    std::swap(data.at(0), data.at(data.size() - 1));
    data.pop_back();

    down_heap(0);
}

template <typename T> void fmm::min_heap<T>::down_heap(uint64_t index) {

    for (uint64_t child = min_child(index);
         index < data.size() && data.at(child).value < data.at(index).value;
         index = child, child = min_child(index)) {

        std::swap(data.at(child), data.at(index));
    }
}

template <typename T> void fmm::min_heap<T>::up_heap(uint64_t index) {

    for (uint64_t par = parent(index);
         index > 0 && data.at(index).value < data.at(par).value;
         index = par, par = parent(index)) {

        std::swap(data.at(index), data.at(par));
    }
}

template <typename T> void fmm::min_heap<T>::print() {

    for (auto n = data.begin(); n != data.end(); ++n) {
        std::cout << "(" << n->index << "," << n->value << ") " << std::endl;
    }
    std::cout << "" << std::endl;
}