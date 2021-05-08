cmake_minimum_required(VERSION 3.19)
project(clib_dev)

set(CMAKE_CXX_STANDARD 17) # tests and examples are CPP
set(CMAKE_C_STANDARD C90)  # lib is C

set(CLIB_DEBUG off) # when "on" it shows detailed debugging messages for clib

# Google Test
#============
include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# library
#========

add_subdirectory(./src)

# Executables
#============

# link & include clib to all targets
include_directories(./src)
link_libraries(clib)

# example_01
add_executable(example_01 examples/example_01.cpp)

# tests api
add_executable(api tests/api.cpp)
target_link_libraries(api gtest_main)

# tests declare
add_executable(declare tests/declare.cpp)
target_link_libraries(declare gtest_main)

# tests parsing
add_executable(parsing tests/parsing.cpp)
target_link_libraries(parsing gtest_main)

# declare tests (they can be run with ctest)
enable_testing()
add_test(test_api api)
add_test(test_declare declare)
add_test(test_parsing parsing)
add_test(test_example_01a example_01 --help)
add_test(test_example_01b example_01 --hello -h)
