#include <iostream>
#include <cstdlib>

#include <fmm/solver.hpp>

using namespace fmm;

//! Temp debug function
void debug(char const *msg){
    std::cout << msg << "\n" << std::endl;
}

void test_heap(){
    min_heap<int>test(10);
    int temp;

    for(int i = 0; i < 10; i++){
        temp = rand() % 100;

        test.insert_or_update(temp,i);
    }   
    
    debug("heap example");
    test.print();

    test.pop();
    
    debug("same heap after one pop()");
    test.print();
}

int main(){
    
    std::cout << "TEST FUNCTION!" << std::endl;

    test_heap();

} 