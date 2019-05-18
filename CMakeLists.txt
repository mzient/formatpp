cmake_minimum_required(VERSION 3.6)

option(BUILD_FORMATPLUSPLUS_TESTS OFF)
set(CMAKE_CXX_STANDARD 11)

project(formatplusplus)
add_library(formatplusplus INTERFACE)
target_include_directories(formatplusplus INTERFACE include)

if(BUILD_FORMATPLUSPLUS_TESTS)
message("Build Format++ tests")
add_subdirectory(test)
endif()
