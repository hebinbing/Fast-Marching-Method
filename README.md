# Fast-Marching-Method
Fast Marching Method implementation for 2d Eikonal equations in C++.

# Requirements
- Python3
- CMake
- NumPy
- Matplotlib     
- OpenCV
- HDF5 C++ API
- H5py

# Build
```shell
mkdir build && cd build
cmake ..
```
# Run example
```shell
cd examples
chmod +x main_script.py
./main_script.py --help [to see usage]
```

# Debug
```shell
cd build
cmake -DCMAKE_CXX_FLAGS="-DNDEBUG" ..
make
```


# Example
- Shortest path to exit in a maze

<img src="https://github.com/avrocha/Fast-Marching-Method/blob/master/images/github_example.png" width="400" height="400">
