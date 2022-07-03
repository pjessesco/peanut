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

#pragma once

// Standard headers
#include <cmath>
#include <limits>
#include <utility>
#include <iostream>

// Peanut headers

// Dependencies headers

namespace Peanut {
    using Index = unsigned int;
    using Float = float;

    /**
     * @brief Check if given \pval is zero or not.
     * @param[in] val Arithmetic type value which will be checked.
     * @tparam T A arithmetic type of parameter \pval.
     * @return If \pT is floating point type, returns true if \pval is close to zero, false if not.
     *         If \pT is not a floating point type, returns true if \pval is zero, false if not.
     */
    template<typename T>
    bool is_zero(T val) requires std::is_arithmetic_v<T>{
        if constexpr (std::is_floating_point_v<T>){
            return std::fabs(val-static_cast<T>(0)) <= std::numeric_limits<T>::epsilon() ||
                   std::fabs(val-static_cast<T>(0)) < std::numeric_limits<T>::min();
        }
        else{
            return val == static_cast<T>(0);
        }
    }

    /**
     * @brief Compile-time checking structure if given constant is in range.
     * @details constexpr `value` is true if \pstart <= \pvar < \pend, false otherwise.
     * @tparam start Minimum value of the range.
     * @tparam var Constant which will be checked whether in range.
     */
    template <Index start, Index var, Index end>
    struct is_between{
        static constexpr bool value = (start<=var) && (var<end);
    };

    /**
     * @brief Helper variable template for \pis_between<start, var, end>.
     */
    template <Index start, Index var, Index end>
    constexpr bool is_between_v = is_between<start, var, end>::value;

    /**
     * @brief Helper structure for \pfor_()
     */
    template<std::size_t N>
    struct num { static const constexpr auto value = N; };

    /**
     * @brief Function which imitates for loop with a compile-time variable.
     * @param[in] func Callable object which will be called with loop variable.
     * @tparam N Compile time loop variable value.
     * @tparam F Arbitrary type, intended to be callable types (std::function, lambda, etc)
     * @example
     * ```
     * for_<3>([&] (auto i) {
     *     std::cout<<i.value<<std::endl;
     * });
     *
     * // Output
     * // 0
     * // 1
     * // 2
     * ```
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
