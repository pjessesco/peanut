cmake_minimum_required(VERSION 3.21)
project(Peanut)

set(CMAKE_CXX_STANDARD 20)

add_library(Peanut INTERFACE)

if(APPLE)
    option(PEANUT_ENABLE_APPLE_SIMD OFF)
    if(PEANUT_ENABLE_APPLE_SIMD)
        target_compile_definitions(Peanut INTERFACE PEANUT_APPLE_SIMD)
        message(STATUS "Peanut : Apple SIMD is enabled")
    else()
        message(STATUS "Peanut : Apple SIMD is disabled")
    endif()
endif()

target_include_directories(Peanut INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if(TEST)
    add_subdirectory(test)
endif()

if(DOCS)
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
    add_subdirectory(docs)
endif()