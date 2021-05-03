cmake_minimum_required(VERSION 3.19)
project(clib C)

set(CMAKE_C_STANDARD 90)
# set(CMAKE_C_FLAGS_DEBUG -DCLILIB_DEBUG) # to compile clilib with detailed logs in debug only

add_library(clib clib.c clib.h)