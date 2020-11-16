add_library(fmm src/solver.hpp src/solver.cpp)

target_include_directories(fmm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_property(TARGET fmm PROPERTY CXX_STANDARD 11)