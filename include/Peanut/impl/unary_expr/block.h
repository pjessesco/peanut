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
     * @brief Expression class which represents a block submatrix.
     * @tparam row_start Lower row index of the block
     * @tparam col_start Lower column index of the block
     * @tparam row_size Row size of the block
     * @tparam col_size Column size of the block
     * @tparam E Matrix expression type.
     */
    template<Index row_start, Index col_start, Index row_size, Index col_size, typename E>
        requires is_matrix_v<E> && is_between_v<0, row_start, E::Row> && is_between_v<0, col_start, E::Col> &&
                 is_between_v<0, row_start + row_size, E::Row + 1> && is_between_v<0, col_start + col_size, E::Col + 1>
    struct MatrixBlock : public MatrixExpr<MatrixBlock<row_start, col_start, row_size, col_size, E>> {
        using Type = typename E::Type;
        MatrixBlock(const E &x) : x{x} {}

        // Static polymorphism implementation of MatrixExpr
        INLINE auto operator()(Index r, Index c) const {
            return x(row_start + r, col_start + c);
        }

        static constexpr Index Row = row_size;
        static constexpr Index Col = col_size;

        void eval(Matrix<Type, Row, Col> &_result) const {
            for (int i=0;i<Row;i++) {
                for (int j=0;j<Col;j++) {
                    _result(i,j) = x(row_start + i, col_start + j);
                }
            }
        }

        const E &x;
    };
}

namespace Peanut {
    /**
     * @brief Block extraction for small result matrices (eager evaluation)
     */
    template<Index row_start, Index col_start, Index row_size, Index col_size, typename E>
        requires is_matrix_v<E> && is_between_v<0, row_start, E::Row> && is_between_v<0, col_start, E::Col> &&
                 is_between_v<0, row_start + row_size, E::Row + 1> && is_between_v<0, col_start + col_size, E::Col + 1> &&
                 (row_size * col_size <= EAGER_EVAL_THRESHOLD)
    Matrix<typename E::Type, row_size, col_size> Block(const MatrixExpr<E> &x) {
        Matrix<typename E::Type, E::Row, E::Col> x_eval = static_cast<const E&>(x);
        Matrix<typename E::Type, row_size, col_size> result;
        for (Index i = 0; i < row_size; i++) {
            for (Index j = 0; j < col_size; j++) {
                result.m_data[i * col_size + j] = x_eval.m_data[(row_start + i) * E::Col + (col_start + j)];
            }
        }
        return result;
    }

    /**
     * @brief Block extraction for large result matrices (lazy evaluation)
     */
    template<Index row_start, Index col_start, Index row_size, Index col_size, typename E>
        requires is_matrix_v<E> && is_between_v<0, row_start, E::Row> && is_between_v<0, col_start, E::Col> &&
                 is_between_v<0, row_start + row_size, E::Row + 1> && is_between_v<0, col_start + col_size, E::Col + 1> &&
                 (row_size * col_size > EAGER_EVAL_THRESHOLD)
    Impl::MatrixBlock<row_start, col_start, row_size, col_size, E> Block(const MatrixExpr<E> &x) {
        return Impl::MatrixBlock<row_start, col_start, row_size, col_size, E>(static_cast<const E &>(x));
    }
}
