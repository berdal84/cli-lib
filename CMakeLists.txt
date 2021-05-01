cmake_minimum_required(VERSION 3.19)
project(example)

set(CMAKE_CXX_STANDARD 17)

# library
add_subdirectory(./src)
include_directories(./src)
link_libraries(cli-lib)

add_executable(example examples/example_01.cpp)