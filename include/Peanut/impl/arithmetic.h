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

#pragma once

// Standard headers
#include <cmath>
#ifdef PEANUT_APPLE_SIMD
#include <simd/simd.h>
#endif

// Peanut headers


// Dependencies headers


namespace Peanut {

#ifdef PEANUT_APPLE_SIMD
#define NAMESPACE simd
#else
#define NAMESPACE std
#endif

    Float acos(const Float &x) { return NAMESPACE::acos(x); }
    Float sin(const Float &x) { return NAMESPACE::sin(x); }
    Float cos(const Float &x) { return NAMESPACE::cos(x); }
    Float tan(const Float &x) { return NAMESPACE::tan(x); }
    Float atan2(const Float &x, const Float &y) {return NAMESPACE::atan2(x, y);}
    Float atan(const Float &x) { return NAMESPACE::atan(x); }
    Float exp(const Float &x){ return NAMESPACE::exp(x); }
    Float log(const Float &x){ return NAMESPACE::log(x); }


    template <typename T> T min(const T &x, const T &y) { return NAMESPACE::min(x, y); }
    template <typename T> T max(const T &x, const T &y) { return NAMESPACE::max(x, y); }
    template <typename T> T sqrt(const T &x) { return NAMESPACE::sqrt(x); }
    template <typename T> T abs(const T &x) {
#ifdef PEANUT_APPLE_SIMD
        return simd_abs(x);
#else
        return std::abs(x);
#endif
    }

    template <typename T>
    T select(const T &a, const T &b, const Bool &c) {
#ifdef PEANUT_APPLE_SIMD
        return simd_select(a, b, c);
#else
        return c ? a : b;
#endif
    }

}
