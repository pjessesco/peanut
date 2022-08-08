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
#include <Peanut.h>

// Dependencies headers
#include "catch_amalgamated.hpp"


TEST_CASE("Default constructor : zero matrix"){
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

TEST_CASE("Construct using parameter pack"){
    Peanut::Matrix<int, 2, 2> intmat(1,2,3,4);
    CHECK(intmat.elem(0, 0) == 1);
    CHECK(intmat.elem(0, 1) == 2);
    CHECK(intmat.elem(1, 0) == 3);
    CHECK(intmat.elem(1, 1) == 4);

    Peanut::Matrix<float, 2, 2> fltmat{1.1f, 2.2f, 3.3f, 4.4f};
    CHECK(fltmat.elem(0, 0) == 1.1f);
    CHECK(fltmat.elem(0, 1) == 2.2f);
    CHECK(fltmat.elem(1, 0) == 3.3f);
    CHECK(fltmat.elem(1, 1) == 4.4f);
}

TEST_CASE("Construct using std::array"){
    std::array<int, 4> intarr{1,2,3,4};
    Peanut::Matrix<int, 2, 2> intmat{intarr};
    CHECK(intmat.elem(0, 0) == 1);
    CHECK(intmat.elem(0, 1) == 2);
    CHECK(intmat.elem(1, 0) == 3);
    CHECK(intmat.elem(1, 1) == 4);

    std::array<float, 4> fltarr{1.1f, 2.2f, 3.3f, 4.4f};
    Peanut::Matrix<float, 2, 2> fltmat{fltarr};
    CHECK(fltmat.elem(0, 0) == 1.1f);
    CHECK(fltmat.elem(0, 1) == 2.2f);
    CHECK(fltmat.elem(1, 0) == 3.3f);
    CHECK(fltmat.elem(1, 1) == 4.4f);
}

TEST_CASE("Static constructors : zeros()"){
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

TEST_CASE("Static constructors : identity()"){
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

TEST_CASE("Static constructors : from_rows()"){
    SECTION("int matrix"){
        Peanut::Matrix<int, 1, 4> a{1,2,3,4};
        Peanut::Matrix<int, 1, 4> b{5,6,7,8};
        Peanut::Matrix<int, 1, 4> c{9,10,11,12};

        auto introwmat = Peanut::Matrix<int, 3, 4>::from_rows(a, b, c);
        CHECK(introwmat.elem(0, 0) == 1);
        CHECK(introwmat.elem(0, 1) == 2);
        CHECK(introwmat.elem(0, 2) == 3);
        CHECK(introwmat.elem(0, 3) == 4);

        CHECK(introwmat.elem(1, 0) == 5);
        CHECK(introwmat.elem(1, 1) == 6);
        CHECK(introwmat.elem(1, 2) == 7);
        CHECK(introwmat.elem(1, 3) == 8);

        CHECK(introwmat.elem(2, 0) == 9);
        CHECK(introwmat.elem(2, 1) == 10);
        CHECK(introwmat.elem(2, 2) == 11);
        CHECK(introwmat.elem(2, 3) == 12);
    }

    SECTION("float matrix"){
        Peanut::Matrix<float, 1, 3> a{0.1f, 0.2f, 0.3f};
        Peanut::Matrix<float, 1, 3> b{0.4f, 0.5f, 0.6f};
        Peanut::Matrix<float, 1, 3> c{0.7f, 0.8f, 0.9f};
        Peanut::Matrix<float, 1, 3> d{1.0f, 1.1f, 1.2f};

        auto floatrowmat = Peanut::Matrix<float, 4, 3>::from_rows(a,b,c,d);
        CHECK(floatrowmat.elem(0, 0) == Catch::Approx(0.1f));
        CHECK(floatrowmat.elem(0, 1) == Catch::Approx(0.2f));
        CHECK(floatrowmat.elem(0, 2) == Catch::Approx(0.3f));

        CHECK(floatrowmat.elem(1, 0) == Catch::Approx(0.4f));
        CHECK(floatrowmat.elem(1, 1) == Catch::Approx(0.5f));
        CHECK(floatrowmat.elem(1, 2) == Catch::Approx(0.6f));

        CHECK(floatrowmat.elem(2, 0) == Catch::Approx(0.7f));
        CHECK(floatrowmat.elem(2, 1) == Catch::Approx(0.8f));
        CHECK(floatrowmat.elem(2, 2) == Catch::Approx(0.9f));

        CHECK(floatrowmat.elem(3, 0) == Catch::Approx(1.0f));
        CHECK(floatrowmat.elem(3, 1) == Catch::Approx(1.1f));
        CHECK(floatrowmat.elem(3, 2) == Catch::Approx(1.2f));
    }
}

TEST_CASE("Static constructors : from_cols()"){
    SECTION("int matrix"){
        Peanut::Matrix<int, 4, 1> a{1,2,3,4};
        Peanut::Matrix<int, 4, 1> b{5,6,7,8};
        Peanut::Matrix<int, 4, 1> c{9,10,11,12};

        auto introwmat = Peanut::Matrix<int, 4, 3>::from_cols(a, b, c);
        CHECK(introwmat.elem(0, 0) == 1);
        CHECK(introwmat.elem(0, 1) == 5);
        CHECK(introwmat.elem(0, 2) == 9);

        CHECK(introwmat.elem(1, 0) == 2);
        CHECK(introwmat.elem(1, 1) == 6);
        CHECK(introwmat.elem(1, 2) == 10);

        CHECK(introwmat.elem(2, 0) == 3);
        CHECK(introwmat.elem(2, 1) == 7);
        CHECK(introwmat.elem(2, 2) == 11);

        CHECK(introwmat.elem(3, 0) == 4);
        CHECK(introwmat.elem(3, 1) == 8);
        CHECK(introwmat.elem(3, 2) == 12);
    }

    SECTION("float matrix"){
        Peanut::Matrix<float, 3, 1> a{0.1f, 0.2f, 0.3f};
        Peanut::Matrix<float, 3, 1> b{0.4f, 0.5f, 0.6f};
        Peanut::Matrix<float, 3, 1> c{0.7f, 0.8f, 0.9f};
        Peanut::Matrix<float, 3, 1> d{1.0f, 1.1f, 1.2f};

        auto floatrowmat = Peanut::Matrix<float, 3, 4>::from_cols(a,b,c,d);
        CHECK(floatrowmat.elem(0, 0) == Catch::Approx(0.1f));
        CHECK(floatrowmat.elem(0, 1) == Catch::Approx(0.4f));
        CHECK(floatrowmat.elem(0, 2) == Catch::Approx(0.7f));
        CHECK(floatrowmat.elem(0, 3) == Catch::Approx(1.0f));

        CHECK(floatrowmat.elem(1, 0) == Catch::Approx(0.2f));
        CHECK(floatrowmat.elem(1, 1) == Catch::Approx(0.5f));
        CHECK(floatrowmat.elem(1, 2) == Catch::Approx(0.8f));
        CHECK(floatrowmat.elem(1, 3) == Catch::Approx(1.1f));

        CHECK(floatrowmat.elem(2, 0) == Catch::Approx(0.3f));
        CHECK(floatrowmat.elem(2, 1) == Catch::Approx(0.6f));
        CHECK(floatrowmat.elem(2, 2) == Catch::Approx(0.9f));
        CHECK(floatrowmat.elem(2, 3) == Catch::Approx(1.2f));
    }
}

TEST_CASE("elem() : Element getter/setter"){
    SECTION("Square matrix"){
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
    SECTION("Non-square matrix"){
        Peanut::Matrix<int, 4, 1> intmat{1,2,3,4};
        CHECK(intmat.elem(0, 0) == 1);
        CHECK(intmat.elem(1, 0) == 2);
        CHECK(intmat.elem(2, 0) == 3);
        CHECK(intmat.elem(3, 0) == 4);
    }
}

TEST_CASE("Row getter/setter"){
    SECTION("Integer matrix"){
        Peanut::Matrix<int, 4, 2> int_42_mat{1,2,3,4,5,6,7,8};

        SECTION("Row getter"){
            auto r0 = int_42_mat.get_row(0);
            auto r1 = int_42_mat.get_row(1);
            auto r2 = int_42_mat.get_row(2);
            auto r3 = int_42_mat.get_row(3);
            CHECK(r0.elem(0, 0) == 1);
            CHECK(r0.elem(0, 1) == 2);
            CHECK(r1.elem(0, 0) == 3);
            CHECK(r1.elem(0, 1) == 4);
            CHECK(r2.elem(0, 0) == 5);
            CHECK(r2.elem(0, 1) == 6);
            CHECK(r3.elem(0, 0) == 7);
            CHECK(r3.elem(0, 1) == 8);
        }

        SECTION("Row setter"){
            int_42_mat.set_row(2, {10, 11});
            auto r0 = int_42_mat.get_row(0);
            auto r1 = int_42_mat.get_row(1);
            auto r2 = int_42_mat.get_row(2);
            auto r3 = int_42_mat.get_row(3);
            CHECK(r0.elem(0, 0) == 1);
            CHECK(r0.elem(0, 1) == 2);
            CHECK(r1.elem(0, 0) == 3);
            CHECK(r1.elem(0, 1) == 4);
            CHECK(r2.elem(0, 0) == 10);
            CHECK(r2.elem(0, 1) == 11);
            CHECK(r3.elem(0, 0) == 7);
            CHECK(r3.elem(0, 1) == 8);
        }
    }
    SECTION("Float matrix"){
        Peanut::Matrix<float, 2, 4> flt_24_mat{1.1f, 5.5f, 4.4f, 2.2f, 7.7f, 6.6f, 9.9f, 8.8f};

        SECTION("Row getter"){
            auto r0 = flt_24_mat.get_row(0);
            auto r1 = flt_24_mat.get_row(1);
            CHECK(r0.elem(0, 0) == Catch::Approx(1.1f));
            CHECK(r0.elem(0, 1) == Catch::Approx(5.5f));
            CHECK(r0.elem(0, 2) == Catch::Approx(4.4f));
            CHECK(r0.elem(0, 3) == Catch::Approx(2.2f));
            CHECK(r1.elem(0, 0) == Catch::Approx(7.7f));
            CHECK(r1.elem(0, 1) == Catch::Approx(6.6f));
            CHECK(r1.elem(0, 2) == Catch::Approx(9.9f));
            CHECK(r1.elem(0, 3) == Catch::Approx(8.8f));
        }

        SECTION("Row setter"){
            flt_24_mat.set_row(0, {0.1f, 0.2f, 0.3f, 0.4f});
            auto r0 = flt_24_mat.get_row(0);
            auto r1 = flt_24_mat.get_row(1);
            CHECK(r0.elem(0, 0) == Catch::Approx(0.1f));
            CHECK(r0.elem(0, 1) == Catch::Approx(0.2f));
            CHECK(r0.elem(0, 2) == Catch::Approx(0.3f));
            CHECK(r0.elem(0, 3) == Catch::Approx(0.4f));
            CHECK(r1.elem(0, 0) == Catch::Approx(7.7f));
            CHECK(r1.elem(0, 1) == Catch::Approx(6.6f));
            CHECK(r1.elem(0, 2) == Catch::Approx(9.9f));
            CHECK(r1.elem(0, 3) == Catch::Approx(8.8f));
        }
    }
}

TEST_CASE("Col getter/setter"){
    SECTION("Integer matrix"){
        Peanut::Matrix<int, 4, 2> int_42_mat{1,2,
                                             3,4,
                                             5,6,
                                             7,8};

        SECTION("Col getter"){
            auto c0 = int_42_mat.get_col(0);
            auto c1 = int_42_mat.get_col(1);
            CHECK(c0.elem(0, 0) == 1);
            CHECK(c0.elem(1, 0) == 3);
            CHECK(c0.elem(2, 0) == 5);
            CHECK(c0.elem(3, 0) == 7);

            CHECK(c1.elem(0, 0) == 2);
            CHECK(c1.elem(1, 0) == 4);
            CHECK(c1.elem(2, 0) == 6);
            CHECK(c1.elem(3, 0) == 8);
        }

        SECTION("Col setter"){
            int_42_mat.set_col(1, {10, 11, 12, 13});
            auto c0 = int_42_mat.get_col(0);
            auto c1 = int_42_mat.get_col(1);
            CHECK(c0.elem(0, 0) == 1);
            CHECK(c0.elem(1, 0) == 3);
            CHECK(c0.elem(2, 0) == 5);
            CHECK(c0.elem(3, 0) == 7);

            CHECK(c1.elem(0, 0) == 10);
            CHECK(c1.elem(1, 0) == 11);
            CHECK(c1.elem(2, 0) == 12);
            CHECK(c1.elem(3, 0) == 13);
        }
    }
    SECTION("Float matrix"){
        Peanut::Matrix<float, 2, 4> flt_24_mat{1.1f, 5.5f, 4.4f, 2.2f,
                                               7.7f, 6.6f, 9.9f, 8.8f};

        SECTION("Col getter"){
            auto c0 = flt_24_mat.get_col(0);
            auto c1 = flt_24_mat.get_col(1);
            auto c2 = flt_24_mat.get_col(2);
            auto c3 = flt_24_mat.get_col(3);
            CHECK(c0.elem(0, 0) == Catch::Approx(1.1f));
            CHECK(c0.elem(1, 0) == Catch::Approx(7.7f));

            CHECK(c1.elem(0, 0) == Catch::Approx(5.5f));
            CHECK(c1.elem(1, 0) == Catch::Approx(6.6f));

            CHECK(c2.elem(0, 0) == Catch::Approx(4.4f));
            CHECK(c2.elem(1, 0) == Catch::Approx(9.9f));

            CHECK(c3.elem(0, 0) == Catch::Approx(2.2f));
            CHECK(c3.elem(1, 0) == Catch::Approx(8.8f));
        }

        SECTION("Col setter"){
            flt_24_mat.set_col(1, {0.1f, 0.2f});
            auto r0 = flt_24_mat.get_row(0);
            auto r1 = flt_24_mat.get_row(1);
            CHECK(r0.elem(0, 0) == Catch::Approx(1.1f));
            CHECK(r0.elem(0, 1) == Catch::Approx(0.1f));
            CHECK(r0.elem(0, 2) == Catch::Approx(4.4f));
            CHECK(r0.elem(0, 3) == Catch::Approx(2.2f));
            CHECK(r1.elem(0, 0) == Catch::Approx(7.7f));
            CHECK(r1.elem(0, 1) == Catch::Approx(0.2f));
            CHECK(r1.elem(0, 2) == Catch::Approx(9.9f));
            CHECK(r1.elem(0, 3) == Catch::Approx(8.8f));
        }
    }
}

TEST_CASE("gaussian_elimination"){
    CHECK("TBD");
}

TEST_CASE("Vector-features : operator[]"){
    auto vec1 = Peanut::Matrix<int, 3, 1>{1,2,4};
    auto vec2 = Peanut::Matrix<int, 1, 3>{6,7,8};
    SECTION("return value"){
        CHECK(vec1[0] == 1);
        CHECK(vec1[1] == 2);
        CHECK(vec1[2] == 4);
        CHECK(vec2[0] == 6);
        CHECK(vec2[1] == 7);
        CHECK(vec2[2] == 8);
    }
    SECTION("return reference"){
        vec1[1] = 10;
        vec2[2] = 0;
        CHECK(vec1[0] == 1);
        CHECK(vec1[1] == 10);
        CHECK(vec1[2] == 4);
        CHECK(vec2[0] == 6);
        CHECK(vec2[1] == 7);
        CHECK(vec2[2] == 0);
    }
}

TEST_CASE("Vector-features : dot product, length(), normalize()"){
    // TBD
}

TEST_CASE("Determinant"){
    Peanut::Matrix<int, 2, 2> int_22_mat{1,2,3,4};

    Peanut::Matrix<float, 3, 3> flt_33_mat{1.1f, 5.5f, 4.4f, 2.2f, 7.7f, 6.6f, 9.9f, 8.8f, 3.3f};

    Peanut::Matrix<float, 5, 5> flt_55_mat{10.10f, 9.9f, 24.24f, 5.5f, 23.23f,
                                           18.18f, 6.6f, 17.17f, 12.12f, 20.20f,
                                           4.4f, 25.25f, 8.8f, 19.19f, 3.3f,
                                           21.21f, 15.15f, 14.14f, 7.7f, 11.11f,
                                           16.16f, 1.1f, 22.22f, 13.13f, 2.2f};
    SECTION("Determinant using Gaussian elimination (det())"){
        CHECK(int_22_mat.det() == -2);
        CHECK(flt_33_mat.det() == Catch::Approx(33.275f));
        CHECK(flt_55_mat.det() == Catch::Approx(2237986.3587442965f));
    }
    SECTION("Determinant calculation recursively (det2())"){
        CHECK(int_22_mat.det2() == -2);
        CHECK(flt_33_mat.det2() == Catch::Approx(33.275f));
        CHECK(flt_55_mat.det2() == Catch::Approx(2237986.3587442965f));
    }
}
