#include <iostream>
#include <algorithm>
#include <random>

#include <fmm/solver.hpp>
#include "gtest/gtest.h"

TEST(HeapTests, IsSorted){
    fmm::min_heap<double> h;

    //Insertion test
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> urd(0.0, 100.0);

    for(fmm::index_t i = 0; i < 500; ++i)
    {   
        fmm::point_t p = {i, i};
        
        h.insert_or_update(urd(gen), p);
    }

    ASSERT_EQ(h.size(), 500);

    // Child Parent Test
    std::vector<fmm::data_t> popped_values;
    popped_values.reserve(100);

    for(int i = 499; i > 0; --i){
        auto node_val = h.node_val(i);
        auto par_val = h.node_val((i % 2 == 0) ? i / 2 - 1 : (i - 1) / 2);
        ASSERT_TRUE(node_val >= par_val);
    }

    // Sort test
    for(auto i = 0; i < 500; i++){
        popped_values.push_back(h.min_node_value());
        h.pop();
    }

    ASSERT_EQ(h.size(), 0);
    ASSERT_TRUE(std::is_sorted(popped_values.begin(), popped_values.end()));
}   

TEST(HeapTests, HeapProperties){
    fmm::min_heap<double> h;

    //Insertion test
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> urd(0.0, 100.0);

    for(fmm::index_t i = 0; i < 500; ++i)
    {   
        fmm::point_t p = {i, i};
        
        h.insert_or_update(urd(gen), p);
    }

    // Child Parent Test
    std::vector<fmm::data_t> popped_values;
    popped_values.reserve(100);

    for(int i = 499; i > 0; --i){
        auto node_val = h.node_val(i);
        auto par_val = h.node_val((i % 2 == 0) ? i / 2 - 1 : (i - 1) / 2);
        ASSERT_TRUE(node_val >= par_val);
    }
}

int main(int argc, char** argv){
    
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}