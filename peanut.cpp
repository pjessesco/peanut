#include <iostream>
#include <chrono>

#include "Matrix.h"
#include "eigen/Eigen/Core"
#include "armadillo-code/include/armadillo"

int main() {

    // Peanut benchmarking
    {
        Peanut::Matrix<int, 6, 6> mat1{1,2,3,4,5,6,
                                       7,8,9,0,1,2,
                                       3,4,5,6,7,8,
                                       9,0,1,2,3,4,
                                       5,6,7,8,9,0,
                                       1,2,3,4,5,6};

        auto time1 = std::chrono::system_clock::now();
        for(int i=0;i<1000000;i++){
            auto mat = (mat1+mat1+mat1+mat1+mat1+mat1+mat1).eval();
        }
        auto time2 = std::chrono::system_clock::now();
        std::cout<<"Peanut : "<<std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() << " [micro-s]"<<std::endl;

    }

    // Eigen benchmarking
    {
        Eigen::Matrix<int, 6, 6> mat1;
        mat1<< 1,2,3,4,5,6,
                7,8,9,0,1,2,
                3,4,5,6,7,8,
                9,0,1,2,3,4,
                5,6,7,8,9,0,
                1,2,3,4,5,6;

        auto time1 = std::chrono::system_clock::now();
        for(int i=0;i<1000000;i++){
            auto mat = (mat1+mat1+mat1+mat1+mat1+mat1+mat1).eval();
        }
        auto time2 = std::chrono::system_clock::now();
        std::cout<<"Eigen : "<<std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() << " [micro-s]"<<std::endl;
    }

    // Armadillo benchmarking
    {
        arma::Mat<int>::fixed<6, 6> mat1{1,2,3,4,5,6,
                                         7,8,9,0,1,2,
                                         3,4,5,6,7,8,
                                         9,0,1,2,3,4,
                                         5,6,7,8,9,0,
                                         1,2,3,4,5,6};

        auto time1 = std::chrono::system_clock::now();
        for(int i=0;i<1000000;i++){
            auto mat = (mat1+mat1+mat1+mat1+mat1+mat1+mat1).eval();
        }
        auto time2 = std::chrono::system_clock::now();
        std::cout<<"Armadillo : "<<std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() << " [micro-s]"<<std::endl;
    }


    return 0;
}
