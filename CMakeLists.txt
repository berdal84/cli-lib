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
include_directories(./src)
link_libraries(clib)

# Executables
#============
add_executable(example_01 examples/example_01.cpp)

add_executable(test_01 tests/test_01.cpp)
target_link_libraries(test_01 gtest_main)
target_link_libraries(test_01 clib)

add_test(NAME tests COMMAND test_01)