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
#include <type_traits>

// Peanut headers

// Dependencies headers

namespace Peanut {

    template <typename E> struct MatrixExpr;

    template <typename E>
        struct is_matrix{
        static constexpr bool value = std::is_base_of_v<MatrixExpr<E>, E>;
    };

    template <typename E>
    constexpr bool is_matrix_v = is_matrix<E>::value;

    // =========================================================================

    template <typename E1, typename E2> requires is_matrix_v<E1> && is_matrix_v<E2>
    struct is_equal_size_mat{
        static constexpr bool value = (E1::row() == E2::row() && E1::col() == E2::col());
    };

    template <typename E1, typename E2>
    constexpr bool is_equal_size_mat_v = is_equal_size_mat<E1, E2>::value;

    // =========================================================================

    template <typename E> requires is_matrix_v<E>
    struct is_square{
        static constexpr bool value = E::row() == E::col();
    };

    template <typename E>
    constexpr bool is_square_v = is_square<E>::value;


}