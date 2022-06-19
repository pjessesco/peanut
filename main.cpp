#include <iostream>

#include "include/Matrix.h"

int main() {

    using namespace Peanut;

    std::array<int, 4> tmp{1,2,3,4};
    Matrix<int, 2, 2> wow(tmp);

    Matrix<int, 2, 2> a = Matrix<int, 2, 2>::identity();
    Matrix<int, 2, 2> b = wow;
    Matrix<int, 2, 2> asdf = (a+b)+(b+a);

    auto werwe = asdf - (wow - a);

    asdf.print_mat();
    wow.print_mat();
    a.print_mat();

    Matrix<int, 2, 2> mat = werwe;
    mat.print_mat();
    mat.elem(0, 0);

    mat.elem(1, 1) = 100;
    mat.print_mat();

    std::cout<<"\n======"<<std::endl;
    std::cout<<is_matrix_v<int><<std::endl;
    std::cout<<is_matrix_v<Matrix<int, 2, 2>><<std::endl;
    std::cout<<is_matrix_v<MatrixExpr<int>><<std::endl;

    std::cout<<is_equal_size_mat_v<Matrix<int, 2, 2>, Matrix<int, 2, 2>><<std::endl;
    std::cout<<is_equal_size_mat_v<Matrix<int, 2, 2>, Matrix<int, 2, 3>><<std::endl;

    std::cout<<is_square_v<Matrix<int, 2, 2>><<std::endl;
    std::cout<<is_square_v<Matrix<int, 2, 3>><<std::endl;

    return 0;
}
