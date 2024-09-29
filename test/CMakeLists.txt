cmake_minimum_required(VERSION 3.27)
project(mySTL)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

find_package(benchmark REQUIRED)

include(GoogleTest)
file(GLOB sources CONFIGURE_DEPENDS *.cpp)
foreach (source IN ITEMS ${sources})
    get_filename_component(name "${source}" NAME_WLE)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} GTest::gtest_main)
    gtest_discover_tests(${name})
endforeach()
