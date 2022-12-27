#include <iostream>
#include <chrono>

#define EIGEN_DONT_VECTORIZE

#include <Peanut/Peanut.h>
#include "eigen/Eigen/Core"
#include "armadillo-code/include/armadillo"
#include "bruteforce-matrix/matrix.h"

#include "longmatstr.h"

#define SUM_STR (mat1+mat1)
#define MAT_STR NUMSTR_1000

int main() {
    constexpr int row = 100;
    constexpr int col = 10;
    int iter = 10000;
    // Peanut benchmarking
    {
        Peanut::Matrix<int, row, col> mat1{MAT_STR};
        Peanut::Matrix<int, row, col> mat;

        auto time1 = std::chrono::high_resolution_clock::now();
        for(int i=0;i<iter;i++){
            mat = (SUM_STR).eval();
        }
        auto time2 = std::chrono::high_resolution_clock::now();

        std::cout<<"Peanut : "<<std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() << " [micro-s]"<<std::endl;
        std::cout<<mat.elem(0, 0)<<std::endl;
    }

    // Eigen benchmarking
    if constexpr(row*col<=10000){
        Eigen::Matrix<int, row, col> mat1;
        Eigen::Matrix<int, row, col> mat;
        mat1<< MAT_STR;

        auto time1 = std::chrono::high_resolution_clock::now();
        for(int i=0;i<iter;i++){
            mat = (SUM_STR).eval();
        }
        auto time2 = std::chrono::high_resolution_clock::now();

        std::cout<<"Eigen : "<<std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() << " [micro-s]"<<std::endl;
        std::cout<<mat.coeff(0, 0)<<std::endl;
    }

    // Armadillo benchmarking
    {
        arma::Mat<int>::fixed<row, col> mat1{MAT_STR};
        arma::Mat<int>::fixed<row, col> mat;

        auto time1 = std::chrono::high_resolution_clock::now();
        for(int i=0;i<iter;i++){
            mat = (SUM_STR).eval();
        }
        auto time2 = std::chrono::high_resolution_clock::now();

        std::cout<<"Armadillo : "<<std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() << " [micro-s]"<<std::endl;
        std::cout<<mat(0, 0)<<std::endl;
    }

    // Naive matrix benchmarking without expression template
    {
        MatrixBT<int, row, col> mat1{MAT_STR};

        auto time1 = std::chrono::high_resolution_clock::now();
        for(int i=0;i<iter;i++){
            auto mat = SUM_STR;
        }
        auto time2 = std::chrono::high_resolution_clock::now();
        std::cout<<"Naive : "<<std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() << " [micro-s]"<<std::endl;
    }

    return 0;
}
