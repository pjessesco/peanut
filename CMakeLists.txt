cmake_minimum_required(VERSION 3.21)
project(Peanut)

set(CMAKE_CXX_STANDARD 20)

add_executable(Peanut
    main.cpp
    include/impl/common.h
    include/impl/matrix.h
    include/impl/matrix_binary_op.h
    include/impl/matrix_unary_op.h
    include/Matrix.h
)
