cmake_minimum_required(VERSION 3.0)
project(fmm LANGUAGES CXX)

add_subdirectory(test)

set_property(TARGET test_heap PROPERTY CXX_STANDARD 11)