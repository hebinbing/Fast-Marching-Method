add_library(fmm include/fmm/solver.hpp src/solver.cpp)

# target_include_directories(fmm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_include_directories(
  fmm
  PUBLIC include
  PRIVATE src)

set_property(TARGET fmm PROPERTY CXX_STANDARD 11)