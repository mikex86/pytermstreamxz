cmake_minimum_required(VERSION 3.14)
project(pytermxz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find pybind11
find_package(pybind11 REQUIRED)

# Add the termstreamxz submodule
add_subdirectory(third_party/termstreamxz)

# Create the Python module
pybind11_add_module(pytermxz src/bindings.cpp)

# Link against termstreamxz
target_link_libraries(pytermxz PRIVATE termstreamxz)

# Set properties for the Python module
target_compile_definitions(pytermxz PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO}) 