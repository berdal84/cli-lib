cmake_minimum_required(VERSION 3.19)
project(example)

set(CMAKE_CXX_STANDARD 17)

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

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(test_01 tests/test_01.cpp)
target_link_libraries(test_01 gtest_main)
add_test(NAME tests COMMAND test_01)

# library
#========
add_subdirectory(./src)
include_directories(./src)
link_libraries(cli-lib)

# Executables
#============
add_executable(example_01 examples/example_01.cpp)