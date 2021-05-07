cmake_minimum_required(VERSION 3.19)
project(clib C)

set(CMAKE_C_STANDARD 90)
# set(CMAKE_C_FLAGS_DEBUG -DCLILIB_DEBUG) # to compile clilib with detailed logs in debug only
add_library(clib clib.c clib.h)

set_target_properties(clib PROPERTIES PUBLIC_HEADER clib.h)
install(TARGETS clib
        DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../install
        PUBLIC_HEADER)