cmake_minimum_required(VERSION 3.27)
project(mySTL)

set(CMAKE_CXX_STANDARD 20)

find_package(benchmark REQUIRED)
add_executable(my_vector_benchmark my_vector_benchmark.cpp)
target_link_libraries(my_vector_benchmark benchmark::benchmark)