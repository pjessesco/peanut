//
// This software is released under the MIT license.
//
// Copyright (c) 2022-2024 Jino Park
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
#include <Peanut.h>

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

        CHECK(tmat.Row == 3);
        CHECK(tmat.Col == 2);
    }

    SECTION("Optimization"){
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

TEST_CASE("Test unary operation : Negation"){
    Peanut::Matrix<int, 2, 2> mat{1,2,
                                  3,4};

    SECTION("Validation"){
        Peanut::Matrix<int, 2, 2> n = -mat;
        CHECK(n.elem(0, 0) == -1);
        CHECK(n.elem(0, 1) == -2);
        CHECK(n.elem(1, 0) == -3);
        CHECK(n.elem(1, 1) == -4);

        CHECK(mat.elem(0, 0) == 1);
        CHECK(mat.elem(0, 1) == 2);
        CHECK(mat.elem(1, 0) == 3);
        CHECK(mat.elem(1, 1) == 4);
    }
}

TEST_CASE("Test unary operation : SubMat"){
    Peanut::Matrix<int, 4, 4> mat{1,2,3,4,
                                  5,6,7,8,
                                  9,10,11,12,
                                  13,14,15,16};

    SECTION("Validation"){
        auto d1 = Peanut::SubMat<2, 1>(mat).eval();

        CHECK(d1.elem(0, 0) == 1);
        CHECK(d1.elem(0, 1) == 3);
        CHECK(d1.elem(0, 2) == 4);
        CHECK(d1.elem(1, 0) == 5);
        CHECK(d1.elem(1, 1) == 7);
        CHECK(d1.elem(1, 2) == 8);
        CHECK(d1.elem(2, 0) == 13);
        CHECK(d1.elem(2, 1) == 15);
        CHECK(d1.elem(2, 2) == 16);

        auto d2 = Peanut::SubMat<0, 0>(mat).eval();
        CHECK(d2.elem(0, 0) == 6);
        CHECK(d2.elem(0, 1) == 7);
        CHECK(d2.elem(0, 2) == 8);
        CHECK(d2.elem(1, 0) == 10);
        CHECK(d2.elem(1, 1) == 11);
        CHECK(d2.elem(1, 2) == 12);
        CHECK(d2.elem(2, 0) == 14);
        CHECK(d2.elem(2, 1) == 15);
        CHECK(d2.elem(2, 2) == 16);

        auto d3 = Peanut::SubMat<3, 3>(mat).eval();
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

TEST_CASE("Test unary operation : Cast"){
    Peanut::Matrix<float, 2, 2> mat{1.1f, 2.2f,
                                    3.3f, 4.4f};

    SECTION("Validation"){
        auto intmat = Peanut::Cast<int>(mat).eval();

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

TEST_CASE("Test unary operation : Combination of unary operations (1)"){
    using namespace Peanut;

    Peanut::Matrix<float, 5, 5> test{1.2f, 3.5f, 2.1f, 4.2f, 5.3f,
                                     1.4f, 3.4f, 1.2f, 5.2f, 1.3f,
                                     4.1f, 2.5f, 1.1f, 4.1f, 2.5f,
                                     2.1f, 3.4f, 2.3f, 5.2f, 3.4f,
                                     2.4f, 2.5f, 1.7f, 4.3f, 2.8f};

    SECTION("Validation"){

        // T
        // 1.2 1.4 4.1 2.1 2.4
        // 3.5 3.4 2.5 3.4 2.5
        // 2.1 1.2 1.1 2.3 1.7
        // 4.2 5.2 4.1 5.2 4.3
        // 5.3 1.3 2.5 3.4 2.8

        //Block<0, 0, 4, 4>
        // 1.2 1.4 4.1 2.1
        // 3.5 3.4 2.5 3.4
        // 2.1 1.2 1.1 2.3
        // 4.2 5.2 4.1 5.2

        // T
        // 1.2 3.5 2.1 4.2
        // 1.4 3.4 1.2 5.2
        // 4.1 2.5 1.1 4.1
        // 2.1 3.4 2.3 5.2

        // SubMat<1, 2>
        // 1.2 3.5 4.2
        // 4.1 2.5 4.1
        // 2.1 3.4 5.2

        // Block<0, 0, 3, 3>
        // 1.2 3.5 4.2
        // 4.1 2.5 4.1
        // 2.1 3.4 5.2

        // T
        // 1.2 4.1 2.1
        // 3.5 2.5 3.4
        // 4.2 4.1 5.2

        auto result1 = T(Block<0, 0, 3, 3>(SubMat<1, 2>(T(Block<0, 0, 4, 4>(T(test))))));
        CHECK(result1.elem(0, 0) == Catch::Approx(1.2f));
        CHECK(result1.elem(0, 1) == Catch::Approx(4.1f));
        CHECK(result1.elem(0, 2) == Catch::Approx(2.1f));
        CHECK(result1.elem(1, 0) == Catch::Approx(3.5f));
        CHECK(result1.elem(1, 1) == Catch::Approx(2.5f));
        CHECK(result1.elem(1, 2) == Catch::Approx(3.4f));
        CHECK(result1.elem(2, 0) == Catch::Approx(4.2f));
        CHECK(result1.elem(2, 1) == Catch::Approx(4.1f));
        CHECK(result1.elem(2, 2) == Catch::Approx(5.2f));

        auto result2 = T(Block<0, 0, 3, 3>(SubMat<1, 2>(T(Block<0, 0, 4, 4>(T(test)))))).eval();
        CHECK(result2.elem(0, 0) == Catch::Approx(1.2f));
        CHECK(result2.elem(0, 1) == Catch::Approx(4.1f));
        CHECK(result2.elem(0, 2) == Catch::Approx(2.1f));
        CHECK(result2.elem(1, 0) == Catch::Approx(3.5f));
        CHECK(result2.elem(1, 1) == Catch::Approx(2.5f));
        CHECK(result2.elem(1, 2) == Catch::Approx(3.4f));
        CHECK(result2.elem(2, 0) == Catch::Approx(4.2f));
        CHECK(result2.elem(2, 1) == Catch::Approx(4.1f));
        CHECK(result2.elem(2, 2) == Catch::Approx(5.2f));
    }
}

TEST_CASE("Test unary operation : Minor"){
    Peanut::Matrix<float, 5, 5> mat{6.5f, 8.1f, 7.6f, 2.5f, 3.8f,
                                    7.1f, 6.2f, 5.3f, 8.7f, 1.6f,
                                    2.5f, 3.7f, 1.8f, 2.5f, 3.8f,
                                    1.2f, 5.3f, 1.6f, 7.2f, 3.5f,
                                    1.5f, 3.1f, 7.6f, 2.3f, 1.7f};

    Peanut::Matrix<float, 3, 3> mat2{1.2f, 5.3f, 3.1f,
                                     6.5f, 8.1f, 0.2f,
                                     7.6f, 2.5f, 4.8f};

    SECTION("Validation"){
        auto mmat = Peanut::Minor(mat).eval();

        CHECK(mmat.elem(0, 0) == Catch::Approx(-128.631f));
        CHECK(mmat.elem(0, 1) == Catch::Approx(-928.599f));
        CHECK(mmat.elem(0, 2) == Catch::Approx(-122.544f));
        CHECK(mmat.elem(0, 3) == Catch::Approx(389.175f));
        CHECK(mmat.elem(0, 4) == Catch::Approx(-505.453f));

        CHECK(mmat.elem(1, 0) == Catch::Approx(-450.856f));
        CHECK(mmat.elem(1, 1) == Catch::Approx(-436.938f));
        CHECK(mmat.elem(1, 2) == Catch::Approx(-20.5352));
        CHECK(mmat.elem(1, 3) == Catch::Approx(270.564f));
        CHECK(mmat.elem(1, 4) == Catch::Approx(58.9056f));

        CHECK(mmat.elem(2, 0) == Catch::Approx(631.634f));
        CHECK(mmat.elem(2, 1) == Catch::Approx(1261.89f));
        CHECK(mmat.elem(2, 2) == Catch::Approx(28.4836f));
        CHECK(mmat.elem(2, 3) == Catch::Approx(-92.0919f));
        CHECK(mmat.elem(2, 4) == Catch::Approx(1491.83f));

        CHECK(mmat.elem(3, 0) == Catch::Approx(656.451f));
        CHECK(mmat.elem(3, 1) == Catch::Approx(801.415f));
        CHECK(mmat.elem(3, 2) == Catch::Approx(155.246f));
        CHECK(mmat.elem(3, 3) == Catch::Approx(124.839f));
        CHECK(mmat.elem(3, 4) == Catch::Approx(357.043f));

        CHECK(mmat.elem(4, 0) == Catch::Approx(-197.179f));
        CHECK(mmat.elem(4, 1) == Catch::Approx(493.743f));
        CHECK(mmat.elem(4, 2) == Catch::Approx(510.55f));
        CHECK(mmat.elem(4, 3) == Catch::Approx(-152.399f));
        CHECK(mmat.elem(4, 4) == Catch::Approx(268.371f));

        auto mmat2 = Peanut::Minor(mat);

        CHECK(mmat2.elem(0, 0) == Catch::Approx(-128.631f));
        CHECK(mmat2.elem(0, 1) == Catch::Approx(-928.599f));
        CHECK(mmat2.elem(0, 2) == Catch::Approx(-122.544f));
        CHECK(mmat2.elem(0, 3) == Catch::Approx(389.175f));
        CHECK(mmat2.elem(0, 4) == Catch::Approx(-505.453f));

        CHECK(mmat2.elem(1, 0) == Catch::Approx(-450.856f));
        CHECK(mmat2.elem(1, 1) == Catch::Approx(-436.938f));
        CHECK(mmat2.elem(1, 2) == Catch::Approx(-20.5352));
        CHECK(mmat2.elem(1, 3) == Catch::Approx(270.564f));
        CHECK(mmat2.elem(1, 4) == Catch::Approx(58.9056f));

        CHECK(mmat2.elem(2, 0) == Catch::Approx(631.634f));
        CHECK(mmat2.elem(2, 1) == Catch::Approx(1261.89f));
        CHECK(mmat2.elem(2, 2) == Catch::Approx(28.4836f));
        CHECK(mmat2.elem(2, 3) == Catch::Approx(-92.0919f));
        CHECK(mmat2.elem(2, 4) == Catch::Approx(1491.83f));

        CHECK(mmat2.elem(3, 0) == Catch::Approx(656.451f));
        CHECK(mmat2.elem(3, 1) == Catch::Approx(801.415f));
        CHECK(mmat2.elem(3, 2) == Catch::Approx(155.246f));
        CHECK(mmat2.elem(3, 3) == Catch::Approx(124.839f));
        CHECK(mmat2.elem(3, 4) == Catch::Approx(357.043f));

        CHECK(mmat2.elem(4, 0) == Catch::Approx(-197.179f));
        CHECK(mmat2.elem(4, 1) == Catch::Approx(493.743f));
        CHECK(mmat2.elem(4, 2) == Catch::Approx(510.55f));
        CHECK(mmat2.elem(4, 3) == Catch::Approx(-152.399f));
        CHECK(mmat2.elem(4, 4) == Catch::Approx(268.371f));
    }

    SECTION("Validation 2"){
        auto mmat = Minor(Minor(Minor(mat2)));
        CHECK(mmat.elem(0, 0) == Catch::Approx(2.43166e+06f));
        CHECK(mmat.elem(0, 1) == Catch::Approx(1.88045e+06f));
        CHECK(mmat.elem(0, 2) == Catch::Approx(-2.87072e+06f));
        CHECK(mmat.elem(1, 0) == Catch::Approx(1.12079e+06f));
        CHECK(mmat.elem(1, 1) == Catch::Approx(-1.12776e+06f));
        CHECK(mmat.elem(1, 2) == Catch::Approx(-2.36197e+06f));
        CHECK(mmat.elem(2, 0) == Catch::Approx(-1.52375e+06f));
        CHECK(mmat.elem(2, 1) == Catch::Approx(-1.26145e+06f));
        CHECK(mmat.elem(2, 2) == Catch::Approx(-1.56683e+06f));

        auto mmat2 = Minor(Minor(Minor(mat2))).eval();
        CHECK(mmat2.elem(0, 0) == Catch::Approx(2.43166e+06f));
        CHECK(mmat2.elem(0, 1) == Catch::Approx(1.88045e+06f));
        CHECK(mmat2.elem(0, 2) == Catch::Approx(-2.87072e+06f));
        CHECK(mmat2.elem(1, 0) == Catch::Approx(1.12079e+06f));
        CHECK(mmat2.elem(1, 1) == Catch::Approx(-1.12776e+06f));
        CHECK(mmat2.elem(1, 2) == Catch::Approx(-2.36197e+06f));
        CHECK(mmat2.elem(2, 0) == Catch::Approx(-1.52375e+06f));
        CHECK(mmat2.elem(2, 1) == Catch::Approx(-1.26145e+06f));
        CHECK(mmat2.elem(2, 2) == Catch::Approx(-1.56683e+06f));
    }

    SECTION("Combination with other operations"){
        using namespace Peanut;
        auto mat = Matrix<float, 3, 3>{5.3f, 8.7f, 1.6f,
                                       2.5f, 3.7f, 1.8f,
                                       2.5f, 3.8f, 1.2f};

        // Block<0, 0, 3, 3>
        // 5.3f, 8.7f, 1.6f,
        // 2.5f, 3.7f, 1.8f,
        // 2.5f, 3.8f, 1.2f

        // Minor
        // -2.4 -1.5 0.25
        // 4.36 2.36 -1.61
        // 9.74 5.54 -2.14

        // SubMat<1, 1>
        // -2.4 0.25
        // 9.74 -2.14

        // Minor
        // -2.14 9.74
        // 0.25 -2.4

        // T
        // -2.14 0.25
        // 9.74 -2.4

        auto val = T(Minor(SubMat<1, 1>(Minor(Block<0, 0, 3, 3>(mat)))));

        CHECK(val.elem(0, 0) == Catch::Approx(-2.14f));
        CHECK(val.elem(0, 1) == Catch::Approx(0.25f));
        CHECK(val.elem(1, 0) == Catch::Approx(9.74f));
        CHECK(val.elem(1, 1) == Catch::Approx(-2.4f));

        auto val2 = T(Minor(SubMat<1, 1>(Minor(Block<0, 0, 3, 3>(mat))))).eval();

        CHECK(val2.elem(0, 0) == Catch::Approx(-2.14f));
        CHECK(val2.elem(0, 1) == Catch::Approx(0.25f));
        CHECK(val2.elem(1, 0) == Catch::Approx(9.74f));
        CHECK(val2.elem(1, 1) == Catch::Approx(-2.4f));
    }

}

TEST_CASE("Test unary operation : Cofactor"){
    Peanut::Matrix<float, 5, 5> mat{6.5f, 8.1f, 7.6f, 2.5f, 3.8f,
                                    7.1f, 6.2f, 5.3f, 8.7f, 1.6f,
                                    2.5f, 3.7f, 1.8f, 2.5f, 3.8f,
                                    1.2f, 5.3f, 1.6f, 7.2f, 3.5f,
                                    1.5f, 3.1f, 7.6f, 2.3f, 1.7f};

    Peanut::Matrix<float, 3, 3> mat2{1.2f, 5.3f, 3.1f,
                                     6.5f, 8.1f, 0.2f,
                                     7.6f, 2.5f, 4.8f};

    SECTION("Validation"){
        auto mmat = Peanut::Cofactor(mat).eval();

        CHECK(mmat.elem(0, 0) == Catch::Approx(-128.631f));
        CHECK(mmat.elem(0, 1) == Catch::Approx(928.599f));
        CHECK(mmat.elem(0, 2) == Catch::Approx(-122.544f));
        CHECK(mmat.elem(0, 3) == Catch::Approx(-389.175f));
        CHECK(mmat.elem(0, 4) == Catch::Approx(-505.453f));

        CHECK(mmat.elem(1, 0) == Catch::Approx(450.856f));
        CHECK(mmat.elem(1, 1) == Catch::Approx(-436.938f));
        CHECK(mmat.elem(1, 2) == Catch::Approx(20.5352));
        CHECK(mmat.elem(1, 3) == Catch::Approx(270.564f));
        CHECK(mmat.elem(1, 4) == Catch::Approx(-58.9056f));

        CHECK(mmat.elem(2, 0) == Catch::Approx(631.634f));
        CHECK(mmat.elem(2, 1) == Catch::Approx(-1261.89f));
        CHECK(mmat.elem(2, 2) == Catch::Approx(28.4836f));
        CHECK(mmat.elem(2, 3) == Catch::Approx(92.0919f));
        CHECK(mmat.elem(2, 4) == Catch::Approx(1491.83f));

        CHECK(mmat.elem(3, 0) == Catch::Approx(-656.451f));
        CHECK(mmat.elem(3, 1) == Catch::Approx(801.415f));
        CHECK(mmat.elem(3, 2) == Catch::Approx(-155.246f));
        CHECK(mmat.elem(3, 3) == Catch::Approx(124.839f));
        CHECK(mmat.elem(3, 4) == Catch::Approx(-357.043f));

        CHECK(mmat.elem(4, 0) == Catch::Approx(-197.179f));
        CHECK(mmat.elem(4, 1) == Catch::Approx(-493.743f));
        CHECK(mmat.elem(4, 2) == Catch::Approx(510.55f));
        CHECK(mmat.elem(4, 3) == Catch::Approx(152.399f));
        CHECK(mmat.elem(4, 4) == Catch::Approx(268.371f));

        auto mmat2 = Peanut::Cofactor(mat);

        CHECK(mmat2.elem(0, 0) == Catch::Approx(-128.631f));
        CHECK(mmat2.elem(0, 1) == Catch::Approx(928.599f));
        CHECK(mmat2.elem(0, 2) == Catch::Approx(-122.544f));
        CHECK(mmat2.elem(0, 3) == Catch::Approx(-389.175f));
        CHECK(mmat2.elem(0, 4) == Catch::Approx(-505.453f));

        CHECK(mmat2.elem(1, 0) == Catch::Approx(450.856f));
        CHECK(mmat2.elem(1, 1) == Catch::Approx(-436.938f));
        CHECK(mmat2.elem(1, 2) == Catch::Approx(20.5352));
        CHECK(mmat2.elem(1, 3) == Catch::Approx(270.564f));
        CHECK(mmat2.elem(1, 4) == Catch::Approx(-58.9056f));

        CHECK(mmat2.elem(2, 0) == Catch::Approx(631.634f));
        CHECK(mmat2.elem(2, 1) == Catch::Approx(-1261.89f));
        CHECK(mmat2.elem(2, 2) == Catch::Approx(28.4836f));
        CHECK(mmat2.elem(2, 3) == Catch::Approx(92.0919f));
        CHECK(mmat2.elem(2, 4) == Catch::Approx(1491.83f));

        CHECK(mmat2.elem(3, 0) == Catch::Approx(-656.451f));
        CHECK(mmat2.elem(3, 1) == Catch::Approx(801.415f));
        CHECK(mmat2.elem(3, 2) == Catch::Approx(-155.246f));
        CHECK(mmat2.elem(3, 3) == Catch::Approx(124.839f));
        CHECK(mmat2.elem(3, 4) == Catch::Approx(-357.043f));

        CHECK(mmat2.elem(4, 0) == Catch::Approx(-197.179f));
        CHECK(mmat2.elem(4, 1) == Catch::Approx(-493.743f));
        CHECK(mmat2.elem(4, 2) == Catch::Approx(510.55f));
        CHECK(mmat2.elem(4, 3) == Catch::Approx(152.399f));
        CHECK(mmat2.elem(4, 4) == Catch::Approx(268.371f));
    }

    SECTION("Validation 2"){
        auto mmat = Cofactor(Cofactor(Cofactor(mat2)));
        CHECK(mmat.elem(0, 0) == Catch::Approx(2.43166e+06f));
        CHECK(mmat.elem(0, 1) == Catch::Approx(-1.88045e+06f));
        CHECK(mmat.elem(0, 2) == Catch::Approx(-2.87072e+06f));
        CHECK(mmat.elem(1, 0) == Catch::Approx(-1.12079e+06f));
        CHECK(mmat.elem(1, 1) == Catch::Approx(-1.12776e+06f));
        CHECK(mmat.elem(1, 2) == Catch::Approx(2.36197e+06f));
        CHECK(mmat.elem(2, 0) == Catch::Approx(-1.52375e+06f));
        CHECK(mmat.elem(2, 1) == Catch::Approx(1.26145e+06f));
        CHECK(mmat.elem(2, 2) == Catch::Approx(-1.56683e+06f));

        auto mmat2 = Cofactor(Cofactor(Cofactor(mat2))).eval();
        CHECK(mmat2.elem(0, 0) == Catch::Approx(2.43166e+06f));
        CHECK(mmat2.elem(0, 1) == Catch::Approx(-1.88045e+06f));
        CHECK(mmat2.elem(0, 2) == Catch::Approx(-2.87072e+06f));
        CHECK(mmat2.elem(1, 0) == Catch::Approx(-1.12079e+06f));
        CHECK(mmat2.elem(1, 1) == Catch::Approx(-1.12776e+06f));
        CHECK(mmat2.elem(1, 2) == Catch::Approx(2.36197e+06f));
        CHECK(mmat2.elem(2, 0) == Catch::Approx(-1.52375e+06f));
        CHECK(mmat2.elem(2, 1) == Catch::Approx(1.26145e+06f));
        CHECK(mmat2.elem(2, 2) == Catch::Approx(-1.56683e+06f));
    }

    SECTION("Combination with other operations"){
        using namespace Peanut;

        // val
        // 1.2f, 5.3f, 3.1f
        // 6.5f, 8.1f, 0.2f
        // 7.6f, 2.5f, 4.8f

        // Cofactor
        // 38.38 -29.68 -45.31
        // -17.69 -17.8 37.28
        // -24.05 19.91 -24.73

        // SubMat<1, 1>
        // 38.38 -45.31
        // -24.05 -24.73

        // Cofactor
        // -24.73 24.05
        // 45.31 38.38

        // T
        // -24.73 45.31
        // 24.05 38.38

        auto val = T(Cofactor(SubMat<1, 1>(Cofactor(mat2))));

        CHECK(val.elem(0, 0) == Catch::Approx(-24.73f));
        CHECK(val.elem(0, 1) == Catch::Approx(45.31f));
        CHECK(val.elem(1, 0) == Catch::Approx(24.05f));
        CHECK(val.elem(1, 1) == Catch::Approx(38.38f));

        auto val2 = T(Cofactor(SubMat<1, 1>(Cofactor(mat2)))).eval();

        CHECK(val2.elem(0, 0) == Catch::Approx(-24.73f));
        CHECK(val2.elem(0, 1) == Catch::Approx(45.31f));
        CHECK(val2.elem(1, 0) == Catch::Approx(24.05f));
        CHECK(val2.elem(1, 1) == Catch::Approx(38.38f));
    }
}

TEST_CASE("Test unary operation : Adjugate"){
    Peanut::Matrix<float, 5, 5> mat{6.5f, 8.1f, 7.6f, 2.5f, 3.8f,
                                    7.1f, 6.2f, 5.3f, 8.7f, 1.6f,
                                    2.5f, 3.7f, 1.8f, 2.5f, 3.8f,
                                    1.2f, 5.3f, 1.6f, 7.2f, 3.5f,
                                    1.5f, 3.1f, 7.6f, 2.3f, 1.7f};

    Peanut::Matrix<float, 3, 3> mat2{1.2f, 5.3f, 3.1f,
                                     6.5f, 8.1f, 0.2f,
                                     7.6f, 2.5f, 4.8f};

    SECTION("Validation"){
        auto mmat = Peanut::Adjugate(mat).eval();
        CHECK(mmat.elem(0, 0) == Catch::Approx(-128.631f));
        CHECK(mmat.elem(0, 1) == Catch::Approx(450.856f));
        CHECK(mmat.elem(0, 2) == Catch::Approx(631.634f));
        CHECK(mmat.elem(0, 3) == Catch::Approx(-656.451f));
        CHECK(mmat.elem(0, 4) == Catch::Approx(-197.179f));

        CHECK(mmat.elem(1, 0) == Catch::Approx(928.599f));
        CHECK(mmat.elem(1, 1) == Catch::Approx(-436.938f));
        CHECK(mmat.elem(1, 2) == Catch::Approx(-1261.89f));
        CHECK(mmat.elem(1, 3) == Catch::Approx(801.415f));
        CHECK(mmat.elem(1, 4) == Catch::Approx(-493.743f));

        CHECK(mmat.elem(2, 0) == Catch::Approx(-122.544f));
        CHECK(mmat.elem(2, 1) == Catch::Approx(20.5352f));
        CHECK(mmat.elem(2, 2) == Catch::Approx(28.4836f));
        CHECK(mmat.elem(2, 3) == Catch::Approx(-155.246f));
        CHECK(mmat.elem(2, 4) == Catch::Approx(510.55f));

        CHECK(mmat.elem(3, 0) == Catch::Approx(-389.175f));
        CHECK(mmat.elem(3, 1) == Catch::Approx(270.564f));
        CHECK(mmat.elem(3, 2) == Catch::Approx(92.0919f));
        CHECK(mmat.elem(3, 3) == Catch::Approx(124.839f));
        CHECK(mmat.elem(3, 4) == Catch::Approx(152.399f));

        CHECK(mmat.elem(4, 0) == Catch::Approx(-505.453f));
        CHECK(mmat.elem(4, 1) == Catch::Approx(-58.9056f));
        CHECK(mmat.elem(4, 2) == Catch::Approx(1491.83f));
        CHECK(mmat.elem(4, 3) == Catch::Approx(-357.043f));
        CHECK(mmat.elem(4, 4) == Catch::Approx(268.371f));


        auto mmat2 = Peanut::Adjugate(mat);
        CHECK(mmat2.elem(0, 0) == Catch::Approx(-128.631f));
        CHECK(mmat2.elem(0, 1) == Catch::Approx(450.856f));
        CHECK(mmat2.elem(0, 2) == Catch::Approx(631.634f));
        CHECK(mmat2.elem(0, 3) == Catch::Approx(-656.451f));
        CHECK(mmat2.elem(0, 4) == Catch::Approx(-197.179f));

        CHECK(mmat2.elem(1, 0) == Catch::Approx(928.599f));
        CHECK(mmat2.elem(1, 1) == Catch::Approx(-436.938f));
        CHECK(mmat2.elem(1, 2) == Catch::Approx(-1261.89f));
        CHECK(mmat2.elem(1, 3) == Catch::Approx(801.415f));
        CHECK(mmat2.elem(1, 4) == Catch::Approx(-493.743f));

        CHECK(mmat2.elem(2, 0) == Catch::Approx(-122.544f));
        CHECK(mmat2.elem(2, 1) == Catch::Approx(20.5352f));
        CHECK(mmat2.elem(2, 2) == Catch::Approx(28.4836f));
        CHECK(mmat2.elem(2, 3) == Catch::Approx(-155.246f));
        CHECK(mmat2.elem(2, 4) == Catch::Approx(510.55f));

        CHECK(mmat2.elem(3, 0) == Catch::Approx(-389.175f));
        CHECK(mmat2.elem(3, 1) == Catch::Approx(270.564f));
        CHECK(mmat2.elem(3, 2) == Catch::Approx(92.0919f));
        CHECK(mmat2.elem(3, 3) == Catch::Approx(124.839f));
        CHECK(mmat2.elem(3, 4) == Catch::Approx(152.399f));

        CHECK(mmat2.elem(4, 0) == Catch::Approx(-505.453f));
        CHECK(mmat2.elem(4, 1) == Catch::Approx(-58.9056f));
        CHECK(mmat2.elem(4, 2) == Catch::Approx(1491.83f));
        CHECK(mmat2.elem(4, 3) == Catch::Approx(-357.043f));
        CHECK(mmat2.elem(4, 4) == Catch::Approx(268.371f));
    }

    SECTION("Validation 2"){
        auto mmat = Adjugate(Adjugate(Adjugate(mat2)));
        CHECK(mmat.elem(0, 0) == Catch::Approx(2.4316578057367792e+6f));
        CHECK(mmat.elem(0, 1) == Catch::Approx(-1.1207927718468895e+6f));
        CHECK(mmat.elem(0, 2) == Catch::Approx(-1.5237459673780494e+6f));
        CHECK(mmat.elem(1, 0) == Catch::Approx(-1.880448245812079e+6f));
        CHECK(mmat.elem(1, 1) == Catch::Approx( -1.1277620881217995e+6f));
        CHECK(mmat.elem(1, 2) == Catch::Approx( 1.2614462457587097e+6f));
        CHECK(mmat.elem(2, 0) == Catch::Approx(-2.870724731056108e+6f));
        CHECK(mmat.elem(2, 1) == Catch::Approx(2.3619646429876788e+6f));
        CHECK(mmat.elem(2, 2) == Catch::Approx(-1.5668290134411296e+6f));

        auto mmat2 = Adjugate(Adjugate(Adjugate(mat2))).eval();
        CHECK(mmat2.elem(0, 0) == Catch::Approx(2.4316578057367792e+6f));
        CHECK(mmat2.elem(0, 1) == Catch::Approx(-1.1207927718468895e+6f));
        CHECK(mmat2.elem(0, 2) == Catch::Approx(-1.5237459673780494e+6f));
        CHECK(mmat2.elem(1, 0) == Catch::Approx(-1.880448245812079e+6f));
        CHECK(mmat2.elem(1, 1) == Catch::Approx( -1.1277620881217995e+6f));
        CHECK(mmat2.elem(1, 2) == Catch::Approx( 1.2614462457587097e+6f));
        CHECK(mmat2.elem(2, 0) == Catch::Approx(-2.870724731056108e+6f));
        CHECK(mmat2.elem(2, 1) == Catch::Approx(2.3619646429876788e+6f));
        CHECK(mmat2.elem(2, 2) == Catch::Approx(-1.5668290134411296e+6f));
    }
}


TEST_CASE("Test unary operation : Inverse"){
    Peanut::Matrix<float, 5, 5> mat1{6.5f, 8.1f, 7.6f, 2.5f, 3.8f,
                                     7.1f, 6.2f, 5.3f, 8.7f, 1.6f,
                                     2.5f, 3.7f, 1.8f, 2.5f, 3.8f,
                                     1.2f, 5.3f, 1.6f, 7.2f, 3.5f,
                                     1.5f, 3.1f, 7.6f, 2.3f, 1.7f};

    SECTION("Validation"){
        auto inv1 = Peanut::Inverse(mat1).eval();
        CHECK(inv1.elem(0, 0) == Catch::Approx(-0.0449671f));
        CHECK(inv1.elem(0, 1) == Catch::Approx(0.157612f));
        CHECK(inv1.elem(0, 2) == Catch::Approx(0.220808f));
        CHECK(inv1.elem(0, 3) == Catch::Approx(-0.229484f));
        CHECK(inv1.elem(0, 4) == Catch::Approx(-0.0689305f));

        CHECK(inv1.elem(1, 0) == Catch::Approx(0.324622f));
        CHECK(inv1.elem(1, 1) == Catch::Approx(-0.152746f));
        CHECK(inv1.elem(1, 2) == Catch::Approx(-0.441133f));
        CHECK(inv1.elem(1, 3) == Catch::Approx(0.280161f));
        CHECK(inv1.elem(1, 4) == Catch::Approx(-0.172604f));

        CHECK(inv1.elem(2, 0) == Catch::Approx(-0.0428394f));
        CHECK(inv1.elem(2, 1) == Catch::Approx(0.00717875f));
        CHECK(inv1.elem(2, 2) == Catch::Approx(0.00995737f));
        CHECK(inv1.elem(2, 3) == Catch::Approx(-0.0542712f));
        CHECK(inv1.elem(2, 4) == Catch::Approx(0.178479f));

        CHECK(inv1.elem(3, 0) == Catch::Approx(-0.136049f));
        CHECK(inv1.elem(3, 1) == Catch::Approx(0.0945844f));
        CHECK(inv1.elem(3, 2) == Catch::Approx(0.0321937f));
        CHECK(inv1.elem(3, 3) == Catch::Approx(0.0436414f));
        CHECK(inv1.elem(3, 4) == Catch::Approx(0.0532762f));

        CHECK(inv1.elem(4, 0) == Catch::Approx(-0.176698f));
        CHECK(inv1.elem(4, 1) == Catch::Approx(-0.0205924f));
        CHECK(inv1.elem(4, 2) == Catch::Approx(0.521517f));
        CHECK(inv1.elem(4, 3) == Catch::Approx(-0.124816f));
        CHECK(inv1.elem(4, 4) == Catch::Approx(0.0938179f));

        auto inv2 = (Peanut::Inverse(mat1));
        CHECK(inv2.elem(0, 0) == Catch::Approx(-0.0449671f));
        CHECK(inv2.elem(0, 1) == Catch::Approx(0.157612f));
        CHECK(inv2.elem(0, 2) == Catch::Approx(0.220808f));
        CHECK(inv2.elem(0, 3) == Catch::Approx(-0.229484f));
        CHECK(inv2.elem(0, 4) == Catch::Approx(-0.0689305f));

        CHECK(inv2.elem(1, 0) == Catch::Approx(0.324622f));
        CHECK(inv2.elem(1, 1) == Catch::Approx(-0.152746f));
        CHECK(inv2.elem(1, 2) == Catch::Approx(-0.441133f));
        CHECK(inv2.elem(1, 3) == Catch::Approx(0.280161f));
        CHECK(inv2.elem(1, 4) == Catch::Approx(-0.172604f));

        CHECK(inv2.elem(2, 0) == Catch::Approx(-0.0428394f));
        CHECK(inv2.elem(2, 1) == Catch::Approx(0.00717875f));
        CHECK(inv2.elem(2, 2) == Catch::Approx(0.00995737f));
        CHECK(inv2.elem(2, 3) == Catch::Approx(-0.0542712f));
        CHECK(inv2.elem(2, 4) == Catch::Approx(0.178479f));

        CHECK(inv2.elem(3, 0) == Catch::Approx(-0.136049f));
        CHECK(inv2.elem(3, 1) == Catch::Approx(0.0945844f));
        CHECK(inv2.elem(3, 2) == Catch::Approx(0.0321937f));
        CHECK(inv2.elem(3, 3) == Catch::Approx(0.0436414f));
        CHECK(inv2.elem(3, 4) == Catch::Approx(0.0532762f));

        CHECK(inv2.elem(4, 0) == Catch::Approx(-0.176698f));
        CHECK(inv2.elem(4, 1) == Catch::Approx(-0.0205924f));
        CHECK(inv2.elem(4, 2) == Catch::Approx(0.521517f));
        CHECK(inv2.elem(4, 3) == Catch::Approx(-0.124816f));
        CHECK(inv2.elem(4, 4) == Catch::Approx(0.0938179f));
    }
}

