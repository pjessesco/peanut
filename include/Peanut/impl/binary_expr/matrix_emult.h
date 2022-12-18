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
#include <Peanut/impl/common.h>
#include <Peanut/impl/matrix_type_traits.h>

// Dependencies headers

namespace Peanut::Impl {

    /**
     * @brief Expression class which represents `EMult()`,
     *        element-wise multiplication.
     * @tparam E1 Left hand side matrix expression type.
     * @tparam E2 Right hand side matrix expression type.
     */
    template<typename E1, typename E2>
        requires is_equal_size_mat_v<E1, E2>
    struct MatrixEMult : public MatrixExpr<MatrixEMult<E1, E2>> {
        using Type = typename E1::Type;
        MatrixEMult(const E1 &x, const E2 &y) : x{x}, y{y} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const {
            return x.elem(r, c) * y.elem(r, c);
        }

        static constexpr Index Row = E1::Row;
        static constexpr Index Col = E1::Col;

        inline auto eval() const {
            return Matrix<Type, Row, Col>(*this);
        }

        const E1 &x;
        const E2 &y;
    };
}

namespace Peanut {
    /**
     * @brief Element-wise multiplication of matrix. See `Impl::MatrixEMult`
     * @tparam E1 Left hand side matrix expression type.
     * @tparam E2 Right hand side matrix expression type.
     * @return Constructed `Impl::MatrixEMult` instance
     */
    template<typename E1, typename E2>
        requires is_equal_size_mat_v<E1, E2>
    Impl::MatrixEMult<E1, E2> EMult(const MatrixExpr<E1> &x, const MatrixExpr<E2> &y) {
        return Impl::MatrixEMult<E1, E2>(static_cast<const E1 &>(x), static_cast<const E2 &>(y));
    }
}
