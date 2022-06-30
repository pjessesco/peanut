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

// Peanut headers
#include "impl/common.h"
#include "impl/matrix_type_traits.h"

// Dependencies headers

namespace Peanut{

    template<typename T, Index Row, Index Col> requires std::is_arithmetic_v<T> && (Row > 0) && (Col > 0)
    struct Matrix;

    template <typename E1, typename E2> requires is_equal_size_mat_v<E1, E2>
    struct MatrixSubtract : public MatrixExpr<MatrixSubtract<E1, E2>>{
        using Type = typename E1::Type;
        MatrixSubtract(const E1 &x, const E2 &y) : x{x}, y{y} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const{
            return x.elem(r, c) - y.elem(r, c);
        }

        static constexpr Index row = E1::row;
        static constexpr Index col = E1::col;

        inline auto eval() const{
            return Matrix<Type, row, col>(*this);
        }

        const E1 &x;
        const E2 &y;
    };

    template <typename E1, typename E2> requires is_equal_size_mat_v<E1, E2>
    MatrixSubtract<E1, E2> operator-(const MatrixExpr<E1> &x, const MatrixExpr<E2> &y){
        return MatrixSubtract<E1, E2>(static_cast<const E1&>(x), static_cast<const E2&>(y));
    }

}