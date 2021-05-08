message("Clib configuration...")

cmake_minimum_required(VERSION 3.19)
project(clib_library C)

set(CMAKE_C_STANDARD 90)

# library
#--------

add_library(clib clib.c clib.h)

# options
#--------

option(CLIB_DEBUG OFF "Enable debug mode") # declare option, off by default.

if ( CLIB_DEBUG STREQUAL on)
    set(CMAKE_C_FLAGS -DCLIB_DEBUG) # detailed logs
    message("CLIB_DEBUG is on")
else()
    message("CLIB_DEBUG is off. Add set(CLIB_DEBUG on) to turn it on.")
endif()

# install
#--------

set_target_properties(clib PROPERTIES PUBLIC_HEADER clib.h)
install(TARGETS clib
        DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../install
        PUBLIC_HEADER)

message("Clib configuration done.")
