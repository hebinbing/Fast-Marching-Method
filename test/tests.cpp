#include <cstdlib>
#include <iostream>

#include "fmm/solver.hpp"

using namespace fmm;

//! Temp debug function
void debug(char const* msg) { std::cout << msg << "\n" << std::endl; }

void test_heap()
{
    min_heap<int> test(10);
    int temp;

    

    for(int i = 0; i < 10; i++)
    {
        temp = rand() % 100;

        test.insert_or_update(temp, i);
    }

    debug("heap example");
    test.print();

    test.pop();

    debug("same heap after one pop()");
    test.print();

   //----

   gridpoint_t p{ 1, coordinates{1,1}} ;
   std::cout << p.value <<  " / " << p.map_index.first << "," << p.map_index.second << std::endl;
}

int main()
{
    std::cout << "TEST FUNCTION!" << std::endl;

    test_heap();
}