#include "defs.hpp"

namespace fmm
{
    template<typename T>
    class min_heap
    {
      public:
        //! Empty constructor
        min_heap() {}

        //! Given max heap size constructor
        min_heap(size_t max_size);

        //! Returns the min node value
        T const min_node_value()
        {
            if(size() == 0)
                std::cerr << "Heap is empty." << std::endl;

            return data.at(0);
        }

        //! Returns the min node coordinates
        point_t min_node_coordinates()
        {
            if(size() == 0)
                std::cerr << "Heap is empty." << std::endl;

            return coordinates.at(0);
        }

        //! Removes the node with smallest value and returns it coordinates
        void pop();

        //! Inserts a node
        void insert_or_update(T new_value, point_t p);

        //! Returns data vector size
        size_t size() { return data.size(); }

        //! Delete this temp function when finished
        void print();

      private:
        //! Heap alues
        std::vector<T> data;

        //! Heap map coordinates
        std::vector<point_t> coordinates;

        //! Updates the value of an existing node
        void update(T new_value, index_t index);

        //! Bubbles down a node
        void down_heap(int index);

        //! Bubbles up a node
        void up_heap(int index);

        int parent(int index) { return floor((index - 1) / 2); }

        int min_child(int index);
    };

    template<typename T>
    min_heap<T>::min_heap(size_t max_size)
    {
        data.reserve(max_size);
        coordinates.reserve(max_size);
    }

    template<typename T>
    int min_heap<T>::min_child(int index)
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

    template<typename T>
    void min_heap<T>::update(T new_value, index_t index)
    {
        data.at(index) = new_value;

        up_heap(index);
    }

    template<typename T>
    void min_heap<T>::insert_or_update(T new_value, point_t p)
    {
        index_t node_index = 0;

        for(node_index = 0; node_index < data.size(); node_index++)
        {
            if(coordinates.at(node_index)[0] == p[0] &&
               coordinates.at(node_index)[1] == p[1])
            {
                // DO THIS IN SOLVER
                if(new_value < data.at(node_index))
                {
                    update(new_value, node_index);
                }
                return;
            }
        }

        data.push_back(new_value);
        coordinates.push_back(p);

        up_heap(data.size() - 1);
    }

    template<typename T>
    void min_heap<T>::pop()
    {
        if(data.size() == 0)
        {
            std::cerr << "Heap is empty." << std::endl;
            return;
        }

        std::swap(data.at(0), data.at(data.size() - 1));
        std::swap(coordinates.at(0), coordinates.at(coordinates.size() - 1));

        data.pop_back();
        coordinates.pop_back();

        down_heap(0);
    }

    template<typename T>
    void min_heap<T>::down_heap(int index)
    {
        for(uint64_t child = min_child(index);
            index < data.size() && data.at(child) < data.at(index);
            index = child, child = min_child(index))
        {
            std::swap(data.at(child), data.at(index));
            std::swap(coordinates.at(child), coordinates.at(index));
        }
    }

    template<typename T>
    void min_heap<T>::up_heap(int index)
    {
        for(uint64_t par = parent(index);
            index > 0 && data.at(index) < data.at(par);
            index = par, par = parent(index))
        {
            std::swap(data.at(index), data.at(par));
            std::swap(coordinates.at(index), coordinates.at(par));
        }
    }

    template<typename T>
    void min_heap<T>::print()
    {
        if(data.size() == 0)
        {
            std::cerr << "Heap is empty." << std::endl;
            return;
        }

        for(size_t index = 0; index < data.size(); index++)
        {
            std::cout << "Node = " << data.at(index)
                      << " : Coordinates = " << coordinates.at(index)[0] << "|"
                      << coordinates.at(index)[1] << std::endl;
        }
        std::cout << "" << std::endl;
    }
}    // namespace fmm