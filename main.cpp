#include <iostream>

#include "include/Matrix.h"

int main() {

    using namespace Peanut;

    std::array<int, 4> tmp{1,2,3,4};
    Matrix<int, 2, 2> wow(tmp);

    auto a = Matrix<int, 2, 2>::identity();
    auto b = wow;

    Matrix<int, 2, 2> asdf = (a+b)+(b+a);
    asdf.print_mat();


    return 0;
}
