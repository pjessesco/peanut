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

// Peanut headers
#include <Peanut/impl/common.h>
#include <Peanut/impl/matrix_type_traits.h>

// Dependencies headers

namespace Peanut::Impl {

    /**
     * @brief Expression class which represents its negation.
     * @tparam E Matrix expression type.
     */
    template<typename E>
        requires is_matrix_v<E>
    struct MatrixNegation : public MatrixExpr<MatrixNegation<E>> {
        using Type = typename E::Type;
        MatrixNegation(const E &x) : x{x} {}

        // Static polymorphism implementation of MatrixExpr
        INLINE auto operator()(Index r, Index c) const {
            return -x(r, c);
        }

        static constexpr Index Row = E::Row;
        static constexpr Index Col = E::Col;

        void eval(Matrix<Type, Row, Col> &_result) const {
            for (int i=0;i<Row;i++) {
                for (int j=0;j<Col;j++) {
                    _result(i,j) = -x(i, j);
                }
            }
        }

        const E &x;
    };
}

namespace Peanut{
    /**
     * @brief Negation operation of matrix.
     * @tparam E Matrix expression type.
     * @return Constructed `Impl::MatrixNegation` instance
     */
    template<typename E>
        requires is_matrix_v<E>
    Impl::MatrixNegation<E> operator-(const MatrixExpr<E> &x) {
        return Impl::MatrixNegation<E>(static_cast<const E &>(x));
    }

    /**
     * @brief Template specialization of `operator-` which represents a
     *        negation of a negation of a matrix. It is equivalent to
     *        a input of the given parameter (i.e., -(-x) = x), so
     *        it does not construct a `MatrixNegation` instance.
     * @tparam E Matrix expression type.
     * @param x `MatrixInverse<E>` type matrix expression.
     * @return Input of the given parameter `x`
     */
    template<typename E>
        requires is_matrix_v<E>
    const E& operator-(const Impl::MatrixNegation<E> &x) {
        return static_cast<const E &>(x.x);
    }

}
