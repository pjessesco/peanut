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

namespace Peanut::Impl {
    template<typename E1, typename E2>
        requires(E1::col == E2::row)
    struct MatrixMult : public MatrixExpr<MatrixMult<E1, E2>> {
        using Type = typename E1::Type;
        MatrixMult(const E1 &_x, const E2 &_y) : x_eval{_x}, y_eval{_y} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const {
            auto ret = x_eval.elem(r, 0) * y_eval.elem(0, c);
            for (Index i = 1; i < E1::col; i++) {
                ret += x_eval.elem(r, i) * y_eval.elem(i, c);
            }
            return ret;
        }

        static constexpr Index row = E1::row;
        static constexpr Index col = E2::col;

        inline auto eval() const {
            return Peanut::Matrix<Type, row, col>(*this);
        }

        // Specify member type as Matrix for evaluation
        const Peanut::Matrix<Type, E1::row, E1::col> x_eval;
        const Peanut::Matrix<Type, E2::row, E2::col> y_eval;
    };

}

namespace Peanut {
    template<typename E1, typename E2>
        requires(E1::col == E2::row)
    Impl::MatrixMult<E1, E2> operator*(const MatrixExpr<E1> &x, const MatrixExpr<E2> &y) {
        return Impl::MatrixMult<E1, E2>(static_cast<const E1 &>(x), static_cast<const E2 &>(y));
    }
}