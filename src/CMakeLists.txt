cmake_minimum_required(VERSION 3.19)
project(cli-lib C)

set(CMAKE_C_STANDARD 90)

# Comment to disable debug mode
add_compile_options(-DCLILIB_DEBUG)

add_library(cli-lib cli-lib.c cli-lib.h)