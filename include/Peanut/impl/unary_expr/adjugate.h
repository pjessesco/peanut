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
     * @brief Expression class which represents an adjugate matrix.
     * @details Note that `MatrixAdjugate` evaluates its input expression
     *          internally during construction to avoid duplicated calculation.
     * @tparam E Matrix expression type.
     */
    template<typename E>
        requires is_matrix_v<E> && is_square_v<E>
    struct MatrixAdjugate : public MatrixExpr<MatrixAdjugate<E>> {
        using Type = typename E::Type;

        MatrixAdjugate(const E &_x) {
            for_<Row>([&](auto r) {
                for_<Col>([&](auto c) {
                    constexpr Index rv = r.value;
                    constexpr Index cv = c.value;

                    Matrix<Type, Row-1, Col-1> submat;
                    SubMat<rv, cv>(_x).eval(submat);
                    const Type e = submat.det();

                    if constexpr ((rv + cv) % 2 == 0) {
                        mat_eval(cv, rv) = e;
                    }
                    else {
                        mat_eval(cv, rv) = -e;
                    }
                });
            });
        }

        // Static polymorphism implementation of MatrixExpr
        INLINE auto operator()(Index r, Index c) const {
            return mat_eval(r, c);
        }

        static constexpr Index Row = E::Row;
        static constexpr Index Col = E::Col;

        INLINE void eval(Matrix<Type, Row, Col> &_result) const {
            for (int i=0;i<Row;i++) {
                for (int j=0;j<Col;j++) {
                    _result(i,j) = mat_eval(i, j);
                }
            }
        }

        Matrix<Type, Row, Col> mat_eval;
    };
}

namespace Peanut {
    /**
     * @brief Adjugate operation of matrix. See `Impl::MatrixAdjugate`
     *        and https://en.wikipedia.org/wiki/Adjugate_matrix for details.
     * @tparam E Matrix expression type.
     * @return Constructed `Impl::MatrixAdjugate` instance
     */
    template<typename E>
        requires is_matrix_v<E> && is_square_v<E>
    Impl::MatrixAdjugate<E> Adjugate(const MatrixExpr<E> &x) {
        return Impl::MatrixAdjugate<E>(static_cast<const E &>(x));
    }
}
