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
        INLINE auto elem(Index r, Index c) const {
            return x.elem(row_start + r, col_start + c);
        }

        static constexpr Index Row = row_size;
        static constexpr Index Col = col_size;

        INLINE Matrix<Type, Row, Col> eval() const {
            return Matrix<Type, Row, Col>(*this);
        }

        const E &x;
    };
}

namespace Peanut {
    /**
     * @brief Get a submatrix matrix. See `Impl::MatrixBlock`
     * @tparam row_start Lower row index of the block
     * @tparam col_start Lower column index of the block
     * @tparam row_size Row size of the block
     * @tparam col_size Column size of the block
     * @tparam E Matrix expression type.
     * @return Constructed `Impl::MatrixBlock` instance
     *
     *     Matrix<int, 4, 4> mat{1,2,3,4,
     *                           5,6,7,8,
     *                           9,0,1,2,
     *                           3,4,5,6};
     *
     *     Matrix<int, 3, 2> ev = Block<0, 1, 3, 2>(mat11);
     *     // 2 3
     *     // 6 7
     *     // 0 1
     *
     */
    template<Index row_start, Index col_start, Index row_size, Index col_size, typename E>
        requires is_matrix_v<E> && is_between_v<0, row_start, E::Row> && is_between_v<0, col_start, E::Col> &&
                 is_between_v<0, row_start + row_size, E::Row + 1> && is_between_v<0, col_start + col_size, E::Col + 1>
    Impl::MatrixBlock<row_start, col_start, row_size, col_size, E> Block(const MatrixExpr<E> &x) {
        return Impl::MatrixBlock<row_start, col_start, row_size, col_size, E>(static_cast<const E &>(x));
    }
}
