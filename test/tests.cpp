#include <cstdlib>
#include <iostream>

#include "fmm/solver.hpp"

using namespace fmm;

//! Temp debug function
void debug(char const* msg) { std::cout << msg << "\n" << std::endl; }

void test_heap()
{
    min_heap<data_t> test(10);

    for(int i = 0; i < 10; i++)
    {
        data_t temp_value = rand() % 100;
        index_t temp_index = i;
        test.insert_or_update(temp_value, temp_index);
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