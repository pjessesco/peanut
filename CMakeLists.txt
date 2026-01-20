cmake_minimum_required(VERSION 3.21)
project(Peanut)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

if(TEST)
    add_subdirectory(test)
endif()

add_subdirectory(eigen)
add_subdirectory(armadillo-code)

add_executable(PeanutBenchmark peanut.cpp)
target_include_directories(PeanutBenchmark PUBLIC peanut)
target_link_libraries(PeanutBenchmark armadillo)
