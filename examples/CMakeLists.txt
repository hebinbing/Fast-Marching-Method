find_package(HDF5 COMPONENTS C CXX)

if(HDF5_FOUND)
    add_library(hdf5_parser hdf5_parser.hpp hdf5_parser.cpp)
    target_link_libraries(hdf5_parser PUBLIC ${HDF5_LIBRARIES})
    target_include_directories(hdf5_parser PUBLIC ${HDF5_INCLUDE_DIRS})

    add_executable(example_1 example_1.cpp)
    target_link_libraries(example_1 PUBLIC hdf5_parser fmm ${HDF5_LIBRARIES})
    target_include_directories(example_1 PUBLIC ${HDF5_INCLUDE_DIRS})
    set_property(TARGET example_1 PROPERTY CXX_STANDARD 11)
    
endif()