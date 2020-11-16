add_executable(tests tests.cpp)

target_link_libraries(tests PUBLIC fmm)

set_property(TARGET tests PROPERTY CXX_STANDARD 11)