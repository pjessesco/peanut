cmake_minimum_required(VERSION 3.21)
project(Peanut)

set(CMAKE_CXX_STANDARD 20)

add_library(Peanut Peanut.cpp)
include_directories(include)

if(TEST)
    add_subdirectory(test)
endif()

if(DOCS)
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
    add_subdirectory(docs)
endif()