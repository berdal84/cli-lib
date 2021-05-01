cmake_minimum_required(VERSION 3.19)
project(cli-lib)

set(CMAKE_CXX_STANDARD 17)

add_library(cli-lib cli-lib.cpp cli-lib.h)