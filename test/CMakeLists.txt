add_executable(PeanutTest
    catch_amalgamated.hpp
    catch_amalgamated.cpp

    test_matrix.cpp
    benchmark.cpp
    test_matrix_binary_op.cpp
    test_matrix_unary_op.cpp
)

target_link_libraries(PeanutTest Peanut)