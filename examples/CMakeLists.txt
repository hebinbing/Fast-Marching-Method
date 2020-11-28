find_package(HDF5 COMPONENTS C CXX)

if(HDF5_FOUND)

    message(HDF5_VERSION)

    add_library(hdf5_parser hdf5_parser.hpp hdf5_parser.cpp)
    target_link_libraries(hdf5_parser PUBLIC fmm ${HDF5_LIBRARIES})
    target_include_directories(hdf5_parser PUBLIC ${HDF5_INCLUDE_DIRS})
    set_property(TARGET hdf5_parser PROPERTY CXX_STANDARD 11)

    add_executable(main_example main_example.cpp)
    target_link_libraries(main_example PRIVATE hdf5_parser)
    set_property(TARGET main_example PROPERTY CXX_STANDARD 11)
    
endif()