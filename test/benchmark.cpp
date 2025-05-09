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

Peanut::Matrix<float, 4, 4> create_test_matrix44(float val) {
    Peanut::Matrix<float, 4, 4> test;
    for (Peanut::Index r = 0; r < 4; ++r) {
        for (Peanut::Index c = 0; c < 4; ++c) {
            test(r, c) = val++;
        }
    }
    return test;
}

Peanut::Matrix<float, 100, 100> create_test_matrix_100_100(float val) {
    Peanut::Matrix<float, 100, 100> test;
    for (Peanut::Index r = 0; r < 100; ++r) {
        for (Peanut::Index c = 0; c < 100; ++c) {
            test(r, c) = val++;
        }
    }
    return test;
}

Peanut::Matrix<float, 300, 300> create_test_matrix_300_300(float val) {
    Peanut::Matrix<float, 300, 300> test;
    for (Peanut::Index r = 0; r < 300; ++r) {
        for (Peanut::Index c = 0; c < 300; ++c) {
            test(r, c) = val++;
        }
    }
    return test;
}


TEST_CASE("benchmark"){
    auto test = create_test_matrix44(1.0f);
    BENCHMARK("small matrix"){
        Peanut::Matrix<float, 4, 4> ret = test + test - (test * test) * (test + test - test * test);
        return ret;
    };

    auto test2 = create_test_matrix_100_100(1.0f);
    BENCHMARK("middle matrix"){
        Peanut::Matrix<float, 100, 100> ret = test2 + test2 - (test2 * test2) * (test2 + test2 - test2 * test2);
        return ret;
    };

#ifdef __APPLE__
    auto test3 = create_test_matrix_300_300(1.0f);
    BENCHMARK("large matrix"){
        Peanut::Matrix<float, 300, 300> ret = test3 + test3 - (test3 * test3) * (test3 + test3 - test3 * test3);
        return ret;
    };
#endif
}

