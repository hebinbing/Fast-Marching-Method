find_package(GTest REQUIRED)


if(GTEST_FOUND)
    message(GTEST_FOUND)

    include_directories(${GTEST_INCLUDE_DIRS})

    add_executable(test_heap test_heap.cpp)
    target_link_libraries(test_heap fmm ${GTEST_BOTH_LIBRARIES})
    set_property(TARGET test_heap PROPERTY CXX_STANDARD 11)
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -pthread")
    
endif()


