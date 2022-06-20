#include <iostream>

#include "../include/Matrix.h"

int main() {

    using namespace Peanut;

    std::array<int, 4> tmp{1,2,3,4};
    Matrix<int, 2, 2> wow(tmp);

    Matrix<int, 2, 2> a = Matrix<int, 2, 2>::identity();
    Matrix<int, 2, 2> b = wow;
    Matrix<int, 2, 2> asdf = (a+b)+(b+a);

    auto werwe = asdf - (wow - a);



    Matrix<int, 2, 2> mat = werwe;
    mat.elem(0, 0);



    {


        Matrix<int, 4, 1> qweqwe(tmp);
        qweqwe.print_mat();
        Matrix<int, 1, 4> werwer = T(qweqwe);
        werwer.print_mat();

        Matrix<int, 4, 4> xxx = qweqwe * (T(qweqwe));

        std::array aerwe{1,2,3,4,2,3,4,1,3,4,1,2,4,1,2,3};
        Matrix<int, 4, 4> asasas{aerwe};

        auto wer = asasas.gaussian_elem();
        wer.print_mat();
        std::cout<<wer.det()<<std::endl;

    }


    return 0;
}
