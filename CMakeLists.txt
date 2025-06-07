cmake_minimum_required(VERSION 3.14)
project(pytermstreamxz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 REQUIRED)

add_subdirectory(third_party/termstreamxz)

pybind11_add_module(pytermstreamxz src/bindings.cpp)

target_link_libraries(pytermstreamxz PRIVATE termstreamxz)