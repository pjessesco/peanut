#include <iostream>
#include <chrono>

#define EIGEN_DONT_VECTORIZE

#include <Peanut/Peanut.h>
#include "eigen/Eigen/Core"
#include "armadillo-code/include/armadillo"
#include "bruteforce-matrix/matrix.h"

#include "longmatstr.h"

#define SUM_STR (mat1+mat1+mat1+mat1+mat1+mat1+mat1+mat1)
#define MAT_STR NUMSTR_10000, NUMSTR_10000, NUMSTR_10000, NUMSTR_10000, NUMSTR_10000, NUMSTR_10000

int main() {
    constexpr int row = 300;
    constexpr int col = 200;
    int iter = 100000;
    // Peanut benchmarking
    {
        Peanut::Matrix<int, row, col> mat1{MAT_STR};
        Peanut::Matrix<int, row, col> mat;

        auto time1 = std::chrono::high_resolution_clock::now();
        for(int i=0;i<iter;i++){
            mat = (SUM_STR);
        }
        auto time2 = std::chrono::high_resolution_clock::now();

        std::cout<<"Peanut : "<<std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() << " [micro-s]"<<std::endl;
        std::cout<<mat(0, 0)<<std::endl;
    }


    return 0;
}
