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
#include <limits>
#include <utility>
#include <iostream>

// Peanut headers

// Dependencies headers
#ifdef PEANUT_APPLE_SIMD
#include <simd/simd.h>
#endif


#if defined(_MSC_VER) && !defined(__llvm__) && !defined(__INTEL_COMPILER)
#define INLINE __forceinline
#else
#define INLINE __attribute__((always_inline)) inline
#endif

namespace Peanut {
    constexpr int simd_lane() {
#ifdef PEANUT_APPLE_SIMD
        return 4;
#else
        return 1;
#endif
    }

#ifdef PEANUT_APPLE_SIMD
    static constexpr bool APPLE_SIMD = true;
    using Index = unsigned int;
    using Int = simd_int4;
    using Float = simd_float4;
    using Bool = simd_int4;
#else
    static constexpr bool APPLE_SIMD = false;
    using Index = unsigned int;
    using Int = int;
    using Float = float;
    using Bool = bool;
#endif

    /**
     * @brief Check if given \p val is zero or not.
     * @param[in] val Arithmetic type value which will be checked.
     * @tparam T A arithmetic type of parameter \p val.
     * @return If \p T is floating point type, returns true if \p val is close to zero, false if not.
     *         If \p T is not a floating point type, returns true if \p val is zero, false if not.
     */

#ifdef PEANUT_APPLE_SIMD
#define pn_any(x) simd_any(x)
#define pn_all(x) simd_all(x)
#define pn_none_of(x) !simd_any(x)
#else
#define pn_any(x) x
#define pn_all(x) x
#define pn_none_of(x) !x
#endif

    template<typename T>
#ifdef PEANUT_APPLE_SIMD
    constexpr bool IS_ARITHMETIC_V = std::is_arithmetic_v<T> || std::is_same_v<T, simd_int4> || std::is_same_v<T, simd_float4>;
#else
    constexpr bool IS_ARITHMETIC_V = std::is_arithmetic_v<T>;
#endif


    template <typename T>
#ifdef PEANUT_APPLE_SIMD
    constexpr bool IS_FLOATING_POINT_V = std::is_floating_point_v<T> || std::is_same_v<T, simd_float4>;
#else
    constexpr bool IS_FLOATING_POINT_V = std::is_floating_point_v<T>;

#endif

    template <typename T>
    Bool is_zero(T val) {
#ifdef PEANUT_APPLE_SIMD
        return simd::abs(val) <= 1e-6;
#else
        return std::fabs(val) <= std::numeric_limits<T>::epsilon();
#endif
    }

    template <typename T>
    Bool is_epsilon_equal(const T &v1, const T &v2, const T &epsilon=Float(1e-5f)) {
#ifdef PEANUT_APPLE_SIMD
        return simd_abs(v1 - v2) <= epsilon;
#else
        return std::fabs(v1 - v2) <= epsilon;
#endif
    }

    /**
     * @brief Compile-time checking structure if given constant is in range.
     * @details constexpr `value` is true if \p start <= \p var < \p end, false otherwise.
     * @tparam start Minimum value of the range.
     * @tparam var Constant which will be checked whether in range.
     */
    template <Index start, Index var, Index end>
    struct is_between{
        static constexpr bool value = (start<=var) && (var<end);
    };

    /**
     * @brief Helper variable template for `is_between<start, var, end>`.
     */
    template <Index start, Index var, Index end>
    constexpr bool is_between_v = is_between<start, var, end>::value;

    /**
     * @brief Helper structure for `for_()`
     */
    template<std::size_t N>
    struct num { static const constexpr auto value = N; };

    /**
     * @brief Function which imitates for loop with a compile-time variable.
     * @param[in] func Callable object which will be called with loop variable.
     * @tparam N Compile time loop variable value.
     * @tparam F Arbitrary type, intended to be callable types (std::function, lambda, etc)
     */
    template <class F, std::size_t... Is>
    void for_(F func, std::index_sequence<Is...>)
    {
        (func(num<Is>{}), ...);
    }

    template <std::size_t N, typename F>
    void for_(F func)
    {
        for_(func, std::make_index_sequence<N>());
    }

}
