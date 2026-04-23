cmake_minimum_required(VERSION 3.15)
project(tcknvkn_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(tcknvkn_cpp STATIC src/tcknvkn.cpp)
target_include_directories(tcknvkn_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(tcknvkn_cpp_tests tests/test_tcknvkn.cpp)
target_link_libraries(tcknvkn_cpp_tests PRIVATE tcknvkn_cpp)

add_executable(tcknvkn_cpp_example examples/quickstart.cpp)
target_link_libraries(tcknvkn_cpp_example PRIVATE tcknvkn_cpp)

enable_testing()
add_test(NAME tcknvkn_cpp_tests COMMAND tcknvkn_cpp_tests)
