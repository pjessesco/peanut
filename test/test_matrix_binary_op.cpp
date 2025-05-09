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
#include <Peanut/Peanut.h>

// Dependencies headers
#include "catch_amalgamated.hpp"


TEST_CASE("Test binary operation : Mat + Mat + Mat"){
    SECTION("int matrix"){
        Peanut::Matrix<Peanut::Int, 2, 2> int_22_mat1{1,2,3,4};
        Peanut::Matrix<Peanut::Int, 2, 2> int_22_mat2{6,7,8,9};
        Peanut::Matrix<Peanut::Int, 2, 2> sum_mat = int_22_mat1 + int_22_mat2 + int_22_mat2;

        CHECK(all(sum_mat(0, 0) == 13));
        CHECK(all(sum_mat(0, 1) == 16));
        CHECK(all(sum_mat(1, 0) == 19));
        CHECK(all(sum_mat(1, 1) == 22));
    }

    SECTION("float matrix"){
        Peanut::Matrix<Peanut::Float, 2, 2> flt_22_mat1{1.0f ,2.0f ,3.0f ,4.0f};
        Peanut::Matrix<Peanut::Float, 2, 2> flt_22_mat2{6.6f,7.7f,8.8f,9.9f};
        Peanut::Matrix<Peanut::Float, 2, 2> sum_mat2 = flt_22_mat1 + flt_22_mat2 + flt_22_mat2;

        CHECK(all(sum_mat2(0, 0) == 14.2f));
        CHECK(all(sum_mat2(0, 1) == 17.4f));
        CHECK(all(sum_mat2(1, 0) == 20.6f));
        CHECK(all(sum_mat2(1, 1) == 23.8f));
    }
}

TEST_CASE("Test binary operation : Mat - Mat - Mat"){
    SECTION("int matrix"){
        Peanut::Matrix<Peanut::Int, 2, 2> int_22_mat1{1,2,3,4};
        Peanut::Matrix<Peanut::Int, 2, 2> int_22_mat2{6,7,8,9};
        Peanut::Matrix<Peanut::Int, 2, 2> sub_mat = int_22_mat1 - int_22_mat2 - int_22_mat2;

        CHECK(all(sub_mat(0, 0) == -11));
        CHECK(all(sub_mat(0, 1) == -12));
        CHECK(all(sub_mat(1, 0) == -13));
        CHECK(all(sub_mat(1, 1) == -14));
    }

    SECTION("float matrix"){
        Peanut::Matrix<Peanut::Float, 2, 2> flt_22_mat1{1.0f, 2.0f, 3.0f, 4.0f};
        Peanut::Matrix<Peanut::Float, 2, 2> flt_22_mat2{6.6f,7.7f,8.8f,9.9f};
        Peanut::Matrix<Peanut::Float, 2, 2> sum_mat2 = flt_22_mat1 - flt_22_mat2 - flt_22_mat2;

        CHECK(all(sum_mat2(0, 0) == -12.2f));
        CHECK(all(sum_mat2(0, 1) == -13.4f));
        CHECK(all(sum_mat2(1, 0) == -14.6f));
        CHECK(all(Peanut::is_epsilon_equal(sum_mat2(1, 1), Peanut::Float(-15.8f))));
    }
}

TEST_CASE("Test binary operation : Scalar * Mat, Mat * Scalar"){
     SECTION("int matrix * Int"){
        Peanut::Matrix<Peanut::Int, 2, 2> int_22_mat1{1,2,3,4};
        Peanut::Matrix<Peanut::Int, 2, 2> scalar_mat_mul1 = int_22_mat1 * Peanut::Int(3);
        Peanut::Matrix<Peanut::Int, 2, 2> scalar_mat_mul2 = Peanut::Int(3) * int_22_mat1;
        // Build error
        // Peanut::Matrix<Peanut::Float, 2, 2> scalar_mat_mul1 = int_22_mat1 * 3;

        CHECK(all(scalar_mat_mul1(0, 0) == 3));
        CHECK(all(scalar_mat_mul1(0, 1) == 6));
        CHECK(all(scalar_mat_mul1(1, 0) == 9));
        CHECK(all(scalar_mat_mul1(1, 1) == 12));

        CHECK(all(scalar_mat_mul2(0, 0) == 3));
        CHECK(all(scalar_mat_mul2(0, 1) == 6));
        CHECK(all(scalar_mat_mul2(1, 0) == 9));
        CHECK(all(scalar_mat_mul2(1, 1) == 12));
    }

    SECTION("float matrix * float"){
        Peanut::Matrix<Peanut::Float, 2, 2> flt_22_mat1{1.1f,2.2f,3.3f,4.4f};
        Peanut::Matrix<Peanut::Float, 2, 2> scalar_mat_mul1 = flt_22_mat1 * 2.0f;
        // Build error
        // Peanut::Matrix<Peanut::Int, 2, 2> scalar_mat_mul2 = flt_22_mat1 * 2.0f;
        CHECK(all(scalar_mat_mul1(0, 0) == 2.2f));
        CHECK(all(scalar_mat_mul1(0, 1) == 4.4f));
        CHECK(all(scalar_mat_mul1(1, 0) == 6.6f));
        CHECK(all(scalar_mat_mul1(1, 1) == 8.8f));
    }
}

TEST_CASE("Test binary operation : Mat * Mat"){
    SECTION("float matrix"){
        Peanut::Matrix<Peanut::Float, 2, 2> flt_22_mat1{1.0f, 2.0f, 3.0f, 4.0f};
        Peanut::Matrix<Peanut::Float, 2, 2> flt_22_mat2{6.6f,7.7f,8.8f,9.9f};
        Peanut::Matrix<Peanut::Float, 2, 2> mul_mat = flt_22_mat1 * flt_22_mat2;

        CHECK(all(mul_mat(0, 0) == 24.2f));
        CHECK(all(mul_mat(0, 1) == 27.5f));
        CHECK(all(mul_mat(1, 0) == 55.0f));
        CHECK(all(Peanut::is_epsilon_equal(mul_mat(1, 1), Peanut::Float(62.7f), Peanut::Float(1e-5f))));
    }
}

TEST_CASE("Test binary operation : Mat * Mat * Mat"){
    SECTION("float matrix"){
        Peanut::Matrix<Peanut::Float, 2, 2> flt_22_mat1{1.0f, 2.0f, 3.0f, 4.0f};
        Peanut::Matrix<Peanut::Float, 2, 2> flt_22_mat2{6.6f,7.7f,8.8f,9.9f};
        Peanut::Matrix<Peanut::Float, 2, 2> flt_22_mat3{5.5f, 4.4f, 3.3f, 2.2f};
        Peanut::Matrix<Peanut::Float, 2, 2> mul_mat = flt_22_mat1 * flt_22_mat2 * flt_22_mat3;

        CHECK(all(Peanut::is_epsilon_equal(mul_mat(0, 0), Peanut::Float(223.85f))));
        CHECK(all(Peanut::is_epsilon_equal(mul_mat(0, 1), Peanut::Float(166.98f), Peanut::Float(1e-4f))));
        CHECK(all(Peanut::is_epsilon_equal(mul_mat(1, 0), Peanut::Float(509.41f), Peanut::Float(1e-4f))));
        CHECK(all(mul_mat(1, 1) == 379.94f));
    }
}

TEST_CASE("Test binary operation : Element-wise multiply"){
    SECTION("float matrix"){
        Peanut::Matrix<Peanut::Float, 2, 2> flt_22_mat1{1.0f, 2.0f, 3.0f, 4.0f};
        Peanut::Matrix<Peanut::Float, 2, 2> flt_22_mat2{6.6f,7.7f,8.8f,9.9f};
        Peanut::Matrix<Peanut::Float, 2, 2> flt_22_mat3{5.5f, 4.4f, 3.3f, 2.2f};
        Peanut::Matrix<Peanut::Float, 2, 2> mul_mat = flt_22_mat1 % flt_22_mat2 % flt_22_mat3;

        CHECK(all(mul_mat(0, 0) == 36.3f));
        CHECK(all(mul_mat(0, 1) == 67.76f));
        CHECK(all(mul_mat(1, 0) == 87.12f));
        CHECK(all(Peanut::is_epsilon_equal(mul_mat(1, 1), Peanut::Float(87.12f))));
    }
}

TEST_CASE("Test binary operation : Element-wise divide"){
    SECTION("float matrix"){
        Peanut::Matrix<float, 2, 2> flt_22_mat1{1.0f, 2.0f, 3.0f, 4.0f};
        Peanut::Matrix<float, 2, 2> flt_22_mat2{6.6f, 7.7f, 8.8f, 9.9f};
        Peanut::Matrix<float, 2, 2> mul_mat = Peanut::EDiv(flt_22_mat1, flt_22_mat2) ;

        CHECK(mul_mat(0, 0) == Catch::Approx(1.0f/6.6f));
        CHECK(mul_mat(0, 1) == Catch::Approx(2.0f/7.7f));
        CHECK(mul_mat(1, 0) == Catch::Approx(3.0f/8.8f));
        CHECK(mul_mat(1, 1) == Catch::Approx(4.0f/9.9f));
    }
}

TEST_CASE("Test binary operation : Random matrix arithmetic"){
    Peanut::Matrix<Peanut::Float, 4, 4> mat1{1.2f, 5.4f, 3.3f, 6.4f,
                                     1.3f, 2.5f, 7.6f, 9.9f,
                                     3.3f, 6.3f, 2.5f, 1.6f,
                                     7.5f, 6.6f, 1.3f, 5.2f};

    Peanut::Matrix<Peanut::Float, 4, 4> mat2{7.0f, 6.0f, 2.0f, 5.0f,
                                     8.0f, 9.0f, 5.0f, 2.0f,
                                     1.0f, 3.0f, 5.0f, 7.0f,
                                     9.0f, 8.0f, 4.0f, 1.0f};

    Peanut::Matrix<Peanut::Float, 4, 4> mat3{6.9f, 8.1f, 7.2f, 6.3f,
                                     4.9f, 8.7f, 1.2f, 6.9f,
                                     3.8f, 4.7f, 6.1f, 2.9f,
                                     3.4f, 6.1f, 9.2f, 3.1f};

    Peanut::Matrix<Peanut::Float, 4, 4> result = mat3 - (mat1 * (mat3 + mat2 - mat1) * mat2 * (mat2 - mat1 - mat3));

    CHECK(all(Peanut::is_epsilon_equal(result(0, 0), Peanut::Float(10599.5645f))));
    CHECK(all(Peanut::is_epsilon_equal(result(0, 1), Peanut::Float(42363.29f))));
    CHECK(all(result(0, 2) == 44611.7f));
    CHECK(all(Peanut::is_epsilon_equal(result(0, 3), Peanut::Float(61768.0508f))));
    CHECK(all(Peanut::is_epsilon_equal(result(1, 0), Peanut::Float(14197.7305f))));
    CHECK(all(Peanut::is_epsilon_equal(result(1, 1), Peanut::Float(51564.207f))));
    CHECK(all(result(1, 2) == 53526.5f));
    CHECK(all(Peanut::is_epsilon_equal(result(1, 3), Peanut::Float(72157.875f))));
    CHECK(all(Peanut::is_epsilon_equal(result(2, 0), Peanut::Float(8774.53417))));
    CHECK(all(Peanut::is_epsilon_equal(result(2, 1), Peanut::Float(39373.7188f))));
    CHECK(all(Peanut::is_epsilon_equal(result(2, 2), Peanut::Float(42050.4219f))));
    CHECK(all(Peanut::is_epsilon_equal(result(2, 3), Peanut::Float(59595.2578f))));
    CHECK(all(Peanut::is_epsilon_equal(result(3, 0), Peanut::Float(14297.7168f))));
    CHECK(all(Peanut::is_epsilon_equal(result(3, 1), Peanut::Float(61323.625f))));
    CHECK(all(Peanut::is_epsilon_equal(result(3, 2), Peanut::Float(65321.7813f))));
    CHECK(all(Peanut::is_epsilon_equal(result(3, 3), Peanut::Float(91666.5468f))));
}


