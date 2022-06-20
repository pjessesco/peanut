cmake_minimum_required(VERSION 3.21)
project(Peanut)

set(CMAKE_CXX_STANDARD 20)

if(TEST)
    add_subdirectory(test)
endif()

add_library(Peanut peanut.cpp)
target_include_directories(Peanut PUBLIC include)