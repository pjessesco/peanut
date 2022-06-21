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

// Peanut headers

// Dependencies headers

namespace Peanut {
    using Index = unsigned int;
    using Float = float;

    inline const Index rc_to_idx(Index Col, Index r, Index c) {
        return Col*r + c;
    }

    template<typename T>
    bool is_zero(T val){
        if constexpr (std::is_floating_point_v<T>){
            return std::fabs(val-static_cast<T>(0)) <= std::numeric_limits<T>::epsilon() ||
                   std::fabs(val-static_cast<T>(0)) < std::numeric_limits<T>::min();
        }
        else{
            return val == static_cast<T>(0);
        }
    }

    template <Index start, Index var, Index end>
    struct is_between{
        static constexpr bool value = (start<=var) && (var<end);
    };

    template <Index start, Index var, Index end>
    constexpr bool is_between_v = is_between<start, var, end>::value;

    // Compile-time for loop, usage :
    //
    //    Peanut::for_<3>([&] (auto i) {
    //        std::cout<<i.value<<std::endl;
    //    });
    //
    // Output :
    //   0
    //   1
    //   2
    //
    template<std::size_t N>
    struct num { static const constexpr auto value = N; };

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

    // Usage

}
