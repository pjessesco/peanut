//
// This software is released under the MIT license.
//
// Copyright (c) 2022 Jino Park
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

// Standard headers
#include <type_traits>
#include <chrono>
using namespace std::chrono;

// Peanut headers
#include <Matrix.h>

// Dependencies headers
#include "catch_amalgamated.hpp"


TEST_CASE("test_matrix_binary_op"){

    SECTION("Mat + Mat + Mat"){
        std::array<int, 4> intarr1{1,2,3,4};
        Peanut::Matrix<int, 2, 2> int_22_mat1{intarr1};

        std::array<int, 4> intarr2{6,7,8,9};
        Peanut::Matrix<int, 2, 2> int_22_mat2{intarr2};

        Peanut::Matrix<int, 2, 2> sum_mat = int_22_mat1 + int_22_mat2 + int_22_mat2;

        CHECK(sum_mat.elem(0, 0) == 13);
        CHECK(sum_mat.elem(0, 1) == 16);
        CHECK(sum_mat.elem(1, 0) == 19);
        CHECK(sum_mat.elem(1, 1) == 22);

        std::array<float, 4> fltarr1{1,2,3,4};
        Peanut::Matrix<float, 2, 2> flt_22_mat1{fltarr1};

        std::array<float, 4> fltarr2{6.6f,7.7f,8.8f,9.9f};
        Peanut::Matrix<float, 2, 2> flt_22_mat2{fltarr2};

        Peanut::Matrix<float, 2, 2> sum_mat2 = flt_22_mat1 + flt_22_mat2 + flt_22_mat2;

        CHECK(sum_mat2.elem(0, 0) == Catch::Approx(14.2f));
        CHECK(sum_mat2.elem(0, 1) == Catch::Approx(17.4f));
        CHECK(sum_mat2.elem(1, 0) == Catch::Approx(20.6f));
        CHECK(sum_mat2.elem(1, 1) == Catch::Approx(23.8f));
    }

    SECTION("Mat - Mat - Mat"){
        std::array<int, 4> intarr1{1,2,3,4};
        Peanut::Matrix<int, 2, 2> int_22_mat1{intarr1};

        std::array<int, 4> intarr2{6,7,8,9};
        Peanut::Matrix<int, 2, 2> int_22_mat2{intarr2};

        Peanut::Matrix<int, 2, 2> sub_mat = int_22_mat1 - int_22_mat2 - int_22_mat2;

        CHECK(sub_mat.elem(0, 0) == -11);
        CHECK(sub_mat.elem(0, 1) == -12);
        CHECK(sub_mat.elem(1, 0) == -13);
        CHECK(sub_mat.elem(1, 1) == -14);

        std::array<float, 4> fltarr1{1,2,3,4};
        Peanut::Matrix<float, 2, 2> flt_22_mat1{fltarr1};

        std::array<float, 4> fltarr2{6.6f,7.7f,8.8f,9.9f};
        Peanut::Matrix<float, 2, 2> flt_22_mat2{fltarr2};

        Peanut::Matrix<float, 2, 2> sum_mat2 = flt_22_mat1 - flt_22_mat2 - flt_22_mat2;

        CHECK(sum_mat2.elem(0, 0) == Catch::Approx(-12.2f));
        CHECK(sum_mat2.elem(0, 1) == Catch::Approx(-13.4f));
        CHECK(sum_mat2.elem(1, 0) == Catch::Approx(-14.6f));
        CHECK(sum_mat2.elem(1, 1) == Catch::Approx(-15.8f));
    }

    SECTION("Mat * Mat * Mat"){
        std::array<float, 4> fltarr1{1,2,3,4};
        Peanut::Matrix<float, 2, 2> flt_22_mat1{fltarr1};

        std::array<float, 4> fltarr2{6.6f,7.7f,8.8f,9.9f};
        Peanut::Matrix<float, 2, 2> flt_22_mat2{fltarr2};

        std::array<float, 4> fltarr3{5.5f, 4.4f, 3.3f, 2.2f};
        Peanut::Matrix<float, 2, 2> flt_22_mat3{fltarr3};

        Peanut::Matrix<float, 2, 2> mul_mat = flt_22_mat1 * flt_22_mat2 * flt_22_mat3;

        CHECK(mul_mat.elem(0, 0) == Catch::Approx(223.85f));
        CHECK(mul_mat.elem(0, 1) == Catch::Approx(166.98f));
        CHECK(mul_mat.elem(1, 0) == Catch::Approx(509.41f));
        CHECK(mul_mat.elem(1, 1) == Catch::Approx(379.94f));
    }

    SECTION("Random matrix arithmetic"){
        Peanut::Matrix<float, 4, 4> mat1(std::array<float, 16>{1.2f, 5.4f, 3.3f, 6.4f,
                                                               1.3f, 2.5f, 7.6f, 9.9f,
                                                               3.3f, 6.3f, 2.5f, 1.6f,
                                                               7.5f, 6.6f, 1.3f, 5.2f});

        Peanut::Matrix<float, 4, 4> mat2(std::array<float, 16>{7.0f, 6.0f, 2.0f, 5.0f,
                                                               8.0f, 9.0f, 5.0f, 2.0f,
                                                               1.0f, 3.0f, 5.0f, 7.0f,
                                                               9.0f, 8.0f, 4.0f, 1.0f});

        Peanut::Matrix<float, 4, 4> mat3(std::array<float, 16>{6.9f, 8.1f, 7.2f, 6.3f,
                                                               4.9f, 8.7f, 1.2f, 6.9f,
                                                               3.8f, 4.7f, 6.1f, 2.9f,
                                                               3.4f, 6.1f, 9.2f, 3.1f});

        Peanut::Matrix<float, 4, 4> result = mat3 - (mat1 * (mat3 + mat2 - mat1) * mat2 * (mat2 - mat1 - mat3));

        CHECK(result.elem(0, 0) == Catch::Approx(10599.6f));
        CHECK(result.elem(0, 1) == Catch::Approx(42363.3f));
        CHECK(result.elem(0, 2) == Catch::Approx(44611.7f));
        CHECK(result.elem(0, 3) == Catch::Approx(61768.1f));
        CHECK(result.elem(1, 0) == Catch::Approx(14197.7f));
        CHECK(result.elem(1, 1) == Catch::Approx(51564.2f));
        CHECK(result.elem(1, 2) == Catch::Approx(53526.5f));
        CHECK(result.elem(1, 3) == Catch::Approx(72157.9f));
        CHECK(result.elem(2, 0) == Catch::Approx(8774.53f));
        CHECK(result.elem(2, 1) == Catch::Approx(39373.7f));
        CHECK(result.elem(2, 2) == Catch::Approx(42050.4f));
        CHECK(result.elem(2, 3) == Catch::Approx(59595.3f));
        CHECK(result.elem(3, 0) == Catch::Approx(14297.7f));
        CHECK(result.elem(3, 1) == Catch::Approx(61323.6f));
        CHECK(result.elem(3, 2) == Catch::Approx(65321.8f));
        CHECK(result.elem(3, 3) == Catch::Approx(91666.5f));

        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                result.elem(i,j) /= 1000.0f;
            }
        }

        CHECK(result.det() == Catch::Approx(-2.72254839985536151f).epsilon(0.001f));

    }

    SECTION("Compare running time"){

        auto start = high_resolution_clock::now();
        for(int i=0;i<1000000;i++){
            Peanut::Matrix<float, 4, 4> mat1(std::array<float, 16>{1.2f, 5.4f, 3.3f, 6.4f,
                                                                   1.3f, 2.5f, 7.6f, 9.9f,
                                                                   3.3f, 6.3f, 2.5f, 1.6f,
                                                                   7.5f, 6.6f, 1.3f, 5.2f});

            Peanut::Matrix<float, 4, 4> mat2(std::array<float, 16>{7.0f, 6.0f, 2.0f, 5.0f,
                                                                   8.0f, 9.0f, 5.0f, 2.0f,
                                                                   1.0f, 3.0f, 5.0f, 7.0f,
                                                                   9.0f, 8.0f, 4.0f, 1.0f});

            Peanut::Matrix<float, 4, 4> mat3(std::array<float, 16>{6.9f, 8.1f, 7.2f, 6.3f,
                                                                   4.9f, 8.7f, 1.2f, 6.9f,
                                                                   3.8f, 4.7f, 6.1f, 2.9f,
                                                                   3.4f, 6.1f, 9.2f, 3.1f});

            Peanut::Matrix<float, 4, 4> result = mat3 - (mat1 * (mat3 + mat2 - mat1) * mat2 * (mat2 - mat1 - mat3));
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        std::cout<<"running time : "<<duration.count() << std::endl;
    }
}
