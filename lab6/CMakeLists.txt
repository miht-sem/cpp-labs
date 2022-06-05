set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.12) # version can be different
project(DC_3_TESTS) #name of your project
add_subdirectory(googletest) # add googletest subdirectory
include_directories(googletest/include) # this is so we can #include<gtest/gtest.h>
enable_testing()
add_executable(tests tests.cpp) # add this executable
target_link_libraries(tests gtest_main) # link google test to thisexecutable
include(GoogleTest)
gtest_discover_tests(tests)