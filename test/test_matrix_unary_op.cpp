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

// Peanut headers
#include <Matrix.h>

// Dependencies headers
#include "catch_amalgamated.hpp"


TEST_CASE("Test unary operation : T"){
    Peanut::Matrix<int, 2, 3> mat{1,2,3,
                                  4,5,6};

    SECTION("Validation"){
        auto tmat = T(mat).eval();

        CHECK(tmat.elem(0, 0) == 1);
        CHECK(tmat.elem(0, 1) == 4);
        CHECK(tmat.elem(1, 0) == 2);
        CHECK(tmat.elem(1, 1) == 5);
        CHECK(tmat.elem(2, 0) == 3);
        CHECK(tmat.elem(2, 1) == 6);

        CHECK(tmat.row == 3);
        CHECK(tmat.col == 2);
    }

    SECTION("Optimization"){
        CHECK(std::is_same_v<decltype(T(mat)), Peanut::MatrixTranspose<Peanut::Matrix<int, 2, 3>>>);
        CHECK(std::is_same_v<decltype(T(T(mat))), Peanut::Matrix<int, 2, 3>>);
    }
}

TEST_CASE("Test unary operation : Block"){
    Peanut::Matrix<int, 4, 4> mat{1,2,3,4,
                                  5,6,7,8,
                                  9,10,11,12,
                                  13,14,15,16};

    SECTION("Validation"){
        auto b1 = Peanut::Block<0,1,3,2>(mat).eval();
        CHECK(b1.elem(0, 0) == 2);
        CHECK(b1.elem(0, 1) == 3);
        CHECK(b1.elem(1, 0) == 6);
        CHECK(b1.elem(1, 1) == 7);
        CHECK(b1.elem(2, 0) == 10);
        CHECK(b1.elem(2, 1) == 11);

        auto b2 = Peanut::Block<0,0,4,4>(mat).eval();
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                CHECK(mat.elem(i,j) == b2.elem(i,j));
            }
        }

        auto b3 = Peanut::Block<3,3,1,1>(mat).eval();
        CHECK(b3.elem(0,0) == 16);
    }
}

TEST_CASE("Test unary operation : DeleteRC"){
    Peanut::Matrix<int, 4, 4> mat{1,2,3,4,
                                  5,6,7,8,
                                  9,10,11,12,
                                  13,14,15,16};

    SECTION("Validation"){
        auto d1 = Peanut::DeleteRC<2, 1>(mat).eval();

        CHECK(d1.elem(0, 0) == 1);
        CHECK(d1.elem(0, 1) == 3);
        CHECK(d1.elem(0, 2) == 4);
        CHECK(d1.elem(1, 0) == 5);
        CHECK(d1.elem(1, 1) == 7);
        CHECK(d1.elem(1, 2) == 8);
        CHECK(d1.elem(2, 0) == 13);
        CHECK(d1.elem(2, 1) == 15);
        CHECK(d1.elem(2, 2) == 16);

        auto d2 = Peanut::DeleteRC<0, 0>(mat).eval();
        CHECK(d2.elem(0, 0) == 6);
        CHECK(d2.elem(0, 1) == 7);
        CHECK(d2.elem(0, 2) == 8);
        CHECK(d2.elem(1, 0) == 10);
        CHECK(d2.elem(1, 1) == 11);
        CHECK(d2.elem(1, 2) == 12);
        CHECK(d2.elem(2, 0) == 14);
        CHECK(d2.elem(2, 1) == 15);
        CHECK(d2.elem(2, 2) == 16);

        auto d3 = Peanut::DeleteRC<3, 3>(mat).eval();
        CHECK(d3.elem(0, 0) == 1);
        CHECK(d3.elem(0, 1) == 2);
        CHECK(d3.elem(0, 2) == 3);
        CHECK(d3.elem(1, 0) == 5);
        CHECK(d3.elem(1, 1) == 6);
        CHECK(d3.elem(1, 2) == 7);
        CHECK(d3.elem(2, 0) == 9);
        CHECK(d3.elem(2, 1) == 10);
        CHECK(d3.elem(2, 2) == 11);
    }

}

TEST_CASE("Test unary operation : Minor"){
    Peanut::Matrix<int, 3, 3> mat{1,2,3,
                                  4,5,6,
                                  7,8,9};

    SECTION("Validation"){
        auto mmat = Peanut::Minor(mat).eval();

        CHECK(mmat.elem(0, 0) == 5*9-6*8);
        CHECK(mmat.elem(0, 1) == 4*9-6*7);
        CHECK(mmat.elem(0, 2) == 4*8-5*7);
        CHECK(mmat.elem(1, 0) == 2*9-3*8);
        CHECK(mmat.elem(1, 1) == 1*9-3*7);
        CHECK(mmat.elem(1, 2) == 1*8-2*7);
        CHECK(mmat.elem(2, 0) == 2*6-3*5);
        CHECK(mmat.elem(2, 1) == 1*6-3*4);
        CHECK(mmat.elem(2, 2) == 1*5-2*4);
    }
}

TEST_CASE("Test unary operation : Cofactor"){
    Peanut::Matrix<int, 3, 3> mat{1,2,3,
                                  4,5,6,
                                  7,8,9};

    SECTION("Validation"){
        auto mmat = Peanut::Cofactor(mat).eval();

        CHECK(mmat.elem(0, 0) == 5*9-6*8);
        CHECK(mmat.elem(0, 1) == -(4*9-6*7));
        CHECK(mmat.elem(0, 2) == 4*8-5*7);
        CHECK(mmat.elem(1, 0) == -(2*9-3*8));
        CHECK(mmat.elem(1, 1) == 1*9-3*7);
        CHECK(mmat.elem(1, 2) == -(1*8-2*7));
        CHECK(mmat.elem(2, 0) == 2*6-3*5);
        CHECK(mmat.elem(2, 1) == -(1*6-3*4));
        CHECK(mmat.elem(2, 2) == 1*5-2*4);
    }
}

TEST_CASE("Test unary operation : cast_to"){
    Peanut::Matrix<float, 2, 2> mat{1.1f, 2.2f,
                                    3.3f, 4.4f};

    SECTION("Validation"){
        auto intmat = Peanut::cast_to<int>(mat).eval();

        CHECK(intmat.elem(0, 0) != Catch::Approx(1.1f));
        CHECK(intmat.elem(0, 1) != Catch::Approx(2.2f));
        CHECK(intmat.elem(1, 0) != Catch::Approx(3.3f));
        CHECK(intmat.elem(1, 1) != Catch::Approx(4.4f));

        CHECK(intmat.elem(0, 0) == 1);
        CHECK(intmat.elem(0, 1) == 2);
        CHECK(intmat.elem(1, 0) == 3);
        CHECK(intmat.elem(1, 1) == 4);

        CHECK(std::is_same_v<decltype(intmat), Peanut::Matrix<int, 2, 2>>);
    }
}
