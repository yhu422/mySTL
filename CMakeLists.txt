cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mySTL VERSION 1.0)
set(mySTL_VERSION 1.0)

add_subdirectory(test)
enable_testing()