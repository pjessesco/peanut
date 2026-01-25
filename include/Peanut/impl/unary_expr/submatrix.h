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
     * @brief Get a submatrix matrix by excluding row and column in given
     *        indices.
     * @tparam row_ex R index which will be excluded.
     * @tparam col_ex Column index which will be excluded.
     * @tparam E Matrix expression type.
     */
    template<Index row_ex, Index col_ex, typename E>
        requires is_matrix_v<E> && is_between_v<0, row_ex, E::Row> && is_between_v<0, col_ex, E::Col>
    struct MatrixSub : public MatrixExpr<MatrixSub<row_ex, col_ex, E>> {
        using Type = typename E::Type;
        MatrixSub(const E &x) : x{x} {}

        // Static polymorphism implementation of MatrixExpr
        INLINE auto operator()(Index r, Index c) const {
            return x(r < row_ex ? r : r + 1, c < col_ex ? c : c + 1);
        }

        static constexpr Index Row = E::Row - 1;
        static constexpr Index Col = E::Col - 1;

        void eval(Matrix<Type, Row, Col> &_result) const {
            for (int i=0;i<Row;i++) {
                for (int j=0;j<Col;j++) {
                    _result(i,j) = x(i < row_ex ? i : i + 1, j < col_ex ? j : j + 1);
                }
            }
        }

        const E &x;
    };
}

namespace Peanut {
    /**
     * @brief SubMat for small result matrices (eager evaluation)
     */
    template<Index row_ex, Index col_ex, typename E>
        requires is_matrix_v<E> && is_between_v<0, row_ex, E::Row> && is_between_v<0, col_ex, E::Col> &&
                 ((E::Row - 1) * (E::Col - 1) <= EAGER_EVAL_THRESHOLD)
    Matrix<typename E::Type, E::Row - 1, E::Col - 1> SubMat(const MatrixExpr<E> &x) {
        constexpr Index Row = E::Row - 1;
        constexpr Index Col = E::Col - 1;
        Matrix<typename E::Type, E::Row, E::Col> x_eval = static_cast<const E&>(x);
        Matrix<typename E::Type, Row, Col> result;
        for (Index i = 0; i < Row; i++) {
            for (Index j = 0; j < Col; j++) {
                Index src_i = i < row_ex ? i : i + 1;
                Index src_j = j < col_ex ? j : j + 1;
                result.m_data[i * Col + j] = x_eval.m_data[src_i * E::Col + src_j];
            }
        }
        return result;
    }

    /**
     * @brief SubMat for large result matrices (lazy evaluation)
     */
    template<Index row_ex, Index col_ex, typename E>
        requires is_matrix_v<E> && is_between_v<0, row_ex, E::Row> && is_between_v<0, col_ex, E::Col> &&
                 ((E::Row - 1) * (E::Col - 1) > EAGER_EVAL_THRESHOLD)
    Impl::MatrixSub<row_ex, col_ex, E> SubMat(const MatrixExpr<E> &x) {
        return Impl::MatrixSub<row_ex, col_ex, E>(static_cast<const E &>(x));
    }
}
