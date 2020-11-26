/**
 * min_heap class is needed to organize and store the TRIAL values
 * when running the solver.
 **/

#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

#include "defs.hpp"

namespace fmm
{
    template<typename data_t>
    class min_heap
    {
      public:
        //! Min_heap empty constructor
        min_heap() {}

        //! Min_heap constructor
        min_heap(size_t max_size);

        //! Returns the min node value
        data_t const min_node()
        {
            if(size() == 0)
                std::cerr << "Heap is empty." << std::endl;

            return data.at(0);
        }

        //! Inserts a node
        void insert_or_update(data_t new_node_value, point_t map_index);

        //! Removes the node with smallest value
        void pop();

        //! Bubbles down a node
        void down_heap(int index);

        //! Bubbles up a node
        void up_heap(int index);

        //! Returns data vector size
        size_t size() { return data.size(); }

        //! Delete this temp function when finished
        void print();

      private:
        //! Heap alues
        std::vector<data_t> data;

        //! Heap map indexes
        std::vector<point_t> indexes;

        //! Updates the value of an existing node
        void update(data_t new_value, index_t index);

        int parent(int index) { return floor((index - 1) / 2); }

        int min_child(int index);
    };

    template<typename data_t>
    min_heap<data_t>::min_heap(size_t max_size)
    {
        data.reserve(max_size);
        indexes.reserve(max_size);
    }

    template<typename data_t>
    int min_heap<data_t>::min_child(int index)
    {
        if(data.size() <= 2 * index + 1)
        {
            return index;
        }
        else if(data.size() - 1 == 2 * index + 1)
        {
            return 2 * index + 1;
        }
        else
        {
            return (data.at(2 * index + 1) <= data.at(2 * index + 2))
                       ? (2 * index + 1)
                       : (2 * index + 2);
        }
    }

    template<typename data_t>
    void min_heap<data_t>::update(data_t new_value, index_t index)
    {
        data.at(index) = new_value;

        up_heap(index);
    }

    template<typename data_t>
    void min_heap<data_t>::insert_or_update(data_t new_node_value,
                                            point_t map_index)
    {
        index_t node_index = 0;

        for(node_index = 0; node_index < data.size(); node_index++)
        {
            if(indexes.at(node_index)[0] == map_index[0] &&
               indexes.at(node_index)[1] == map_index[1])
            {
                if(new_node_value < data.at(node_index))
                {
                    update(new_node_value, node_index);
                }
                return;
            }
        }

        data.push_back(new_node_value);
        indexes.push_back(map_index);

        up_heap(data.size() - 1);
    }

    template<typename data_t>
    void min_heap<data_t>::pop()
    {
        if(data.size() == 0)
        {
            std::cerr << "Heap is empty." << std::endl;
            return;
        }

        std::swap(data.at(0), data.at(data.size() - 1));
        std::swap(indexes.at(0), indexes.at(indexes.size() - 1));

        data.pop_back();
        indexes.pop_back();

        down_heap(0);
    }

    template<typename data_t>
    void min_heap<data_t>::down_heap(int index)
    {
        for(uint64_t child = min_child(index);
            index < data.size() && data.at(child) < data.at(index);
            index = child, child = min_child(index))
        {
            std::swap(data.at(child), data.at(index));
            std::swap(indexes.at(child), indexes.at(index));
        }
    }

    template<typename data_t>
    void min_heap<data_t>::up_heap(int index)
    {
        for(uint64_t par = parent(index);
            index > 0 && data.at(index) < data.at(par);
            index = par, par = parent(index))
        {
            std::swap(data.at(index), data.at(par));
            std::swap(indexes.at(index), indexes.at(par));
        }
    }

    template<typename data_t>
    void min_heap<data_t>::print()
    {
        if(data.size() == 0)
        {
            std::cerr << "Heap is empty." << std::endl;
            return;
        }

        for(size_t index = 0; index < data.size(); index++)
        {
            std::cout << "VALUE = " << data.at(index)
                      << " : MAP_INDEXES = " << indexes.at(index)[0] << "|"
                      << indexes.at(index)[1] << std::endl;
        }
        std::cout << "" << std::endl;
    }
}    // namespace fmm