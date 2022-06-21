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
#include <array>
#include <vector>

// Peanut headers
#include <Matrix.h>

// Dependencies headers
#include "catch_amalgamated.hpp"


TEST_CASE("Constructing Matrix"){
    SECTION("Default constructor : zero matrix"){
        Peanut::Matrix<int, 2, 2> zero_22_int_mat;
        CHECK(zero_22_int_mat.elem(0, 0) == 0);
        CHECK(zero_22_int_mat.elem(0, 1) == 0);
        CHECK(zero_22_int_mat.elem(1, 0) == 0);
        CHECK(zero_22_int_mat.elem(1, 1) == 0);

        Peanut::Matrix<float, 2, 2> zero_22_float_mat;
        CHECK(zero_22_float_mat.elem(0, 0) == Catch::Approx(0.0f));
        CHECK(zero_22_float_mat.elem(0, 1) == Catch::Approx(0.0f));
        CHECK(zero_22_float_mat.elem(1, 0) == Catch::Approx(0.0f));
        CHECK(zero_22_float_mat.elem(1, 1) == Catch::Approx(0.0f));
    }

    SECTION("Construct using initialize list"){
        CHECK("TBD");
    }

    SECTION("Construct using std::array"){
        std::array<int, 4> intarr{1,2,3,4};
        Peanut::Matrix<int, 2, 2> intmat{intarr};
        CHECK(intmat.elem(0, 0) == 1);
        CHECK(intmat.elem(0, 1) == 2);
        CHECK(intmat.elem(1, 0) == 3);
        CHECK(intmat.elem(1, 1) == 4);

        Peanut::Matrix<int, 2, 2> intmat2(intarr);
        CHECK(intmat2.elem(0, 0) == 1);
        CHECK(intmat2.elem(0, 1) == 2);
        CHECK(intmat2.elem(1, 0) == 3);
        CHECK(intmat2.elem(1, 1) == 4);

        std::array<float, 4> fltarr{1.1f, 2.2f, 3.3f, 4.4f};
        Peanut::Matrix<float, 2, 2> fltmat{fltarr};
        CHECK(fltmat.elem(0, 0) == 1.1f);
        CHECK(fltmat.elem(0, 1) == 2.2f);
        CHECK(fltmat.elem(1, 0) == 3.3f);
        CHECK(fltmat.elem(1, 1) == 4.4f);
    }

    SECTION("Static constructors : zeros()"){
        auto zero_22_int_mat = Peanut::Matrix<int, 2, 2>::zeros();
        CHECK(zero_22_int_mat.elem(0, 0) == 0);
        CHECK(zero_22_int_mat.elem(0, 1) == 0);
        CHECK(zero_22_int_mat.elem(1, 0) == 0);
        CHECK(zero_22_int_mat.elem(1, 1) == 0);

        auto zero_22_float_mat = Peanut::Matrix<float, 2, 2>::zeros();
        CHECK(zero_22_float_mat.elem(0, 0) == Catch::Approx(0.0f));
        CHECK(zero_22_float_mat.elem(0, 1) == Catch::Approx(0.0f));
        CHECK(zero_22_float_mat.elem(1, 0) == Catch::Approx(0.0f));
        CHECK(zero_22_float_mat.elem(1, 1) == Catch::Approx(0.0f));
    }

    SECTION("Static constructors : identity()"){
        auto ident_22_int_mat = Peanut::Matrix<int, 2, 2>::identity();
        CHECK(ident_22_int_mat.elem(0, 0) == 1);
        CHECK(ident_22_int_mat.elem(0, 1) == 0);
        CHECK(ident_22_int_mat.elem(1, 0) == 0);
        CHECK(ident_22_int_mat.elem(1, 1) == 1);

        auto ident_22_float_mat = Peanut::Matrix<float, 2, 2>::identity();
        CHECK(ident_22_float_mat.elem(0, 0) == Catch::Approx(1.0f));
        CHECK(ident_22_float_mat.elem(0, 1) == Catch::Approx(0.0f));
        CHECK(ident_22_float_mat.elem(1, 0) == Catch::Approx(0.0f));
        CHECK(ident_22_float_mat.elem(1, 1) == Catch::Approx(1.0f));
    }
}

TEST_CASE("Matrix features"){
    SECTION("elem() : Element getter/setter"){
        auto intmat = Peanut::Matrix<int, 2, 2>::identity();
        CHECK(intmat.elem(0, 0) == 1);
        CHECK(intmat.elem(0, 1) == 0);
        CHECK(intmat.elem(1, 0) == 0);
        CHECK(intmat.elem(1, 1) == 1);

        intmat.elem(0, 0) = 5;
        intmat.elem(0, 1) = 6;
        intmat.elem(1, 0) = 7;
        intmat.elem(1, 1) = 8;

        CHECK(intmat.elem(0, 0) == 5);
        CHECK(intmat.elem(0, 1) == 6);
        CHECK(intmat.elem(1, 0) == 7);
        CHECK(intmat.elem(1, 1) == 8);
    }

    SECTION("gaussian_elimination"){
        CHECK("TBD");
    }

    SECTION("Determinant"){
        std::array<int, 4> intarr{1,2,3,4};
        Peanut::Matrix<int, 2, 2> int_22_mat{intarr};
        CHECK(int_22_mat.det() == -2);

        std::array<float, 9> fltarr1{1.1f, 5.5f, 4.4f, 2.2f, 7.7f, 6.6f, 9.9f, 8.8f, 3.3f};
        Peanut::Matrix<float, 3, 3> flt_33_mat{fltarr1};
        CHECK(flt_33_mat.det() == Catch::Approx(33.275f));

        std::array<float, 25> fltarr2{
            10.10f, 9.9f, 24.24f, 5.5f, 23.23f,
            18.18f, 6.6f, 17.17f, 12.12f, 20.20f,
            4.4f, 25.25f, 8.8f, 19.19f, 3.3f,
            21.21f, 15.15f, 14.14f, 7.7f, 11.11f,
            16.16f, 1.1f, 22.22f, 13.13f, 2.2f
        };
        Peanut::Matrix<float, 5, 5> flt_55_mat{fltarr2};
        CHECK(flt_55_mat.det() == Catch::Approx(2237986.3587442965f));
    }




}
