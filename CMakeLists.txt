cmake_minimum_required(VERSION 3.21)
project(Peanut)

set(CMAKE_CXX_STANDARD 20)

add_executable(PeanutBenchmark peanut.cpp)
target_include_directories(PeanutBenchmark PUBLIC peanut)

add_subdirectory(eigen)
add_subdirectory(armadillo-code)
