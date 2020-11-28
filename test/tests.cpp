#include <cstdlib>
#include <iostream>

#include "fmm/solver.hpp"

using namespace fmm;

//! Temp debug function
void debug(char const* msg) { std::cout << msg << "\n" << std::endl; }

void test_heap()
{
    min_heap<data_t> test(10);

    for(index_t x = 0, i = 0; i < 10; i++, x++)
    {
        data_t temp_value = rand() % 100;
        point_t temp_map_index = { x, x };
        test.insert_or_update(temp_value, temp_map_index);
    }

    debug("heap example");
    test.print();

    test.pop();

    debug("same heap after one pop()");
    test.print();
}

int main()
{
    std::cout << "TEST FUNCTION!" << std::endl;

    test_heap();
}