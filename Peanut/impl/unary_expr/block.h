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

    template <Index row_start, Index col_start, Index row_size, Index col_size, typename E>
        requires is_matrix_v<E> && is_between_v<0, row_start, E::row> && is_between_v<0, col_start, E::col> &&
                 is_between_v<0, row_start+row_size, E::row+1> && is_between_v<0, col_start+col_size, E::col+1>
    struct MatrixBlock : public MatrixExpr<MatrixBlock<row_start, col_start, row_size, col_size, E>>{
        using Type = typename E::Type;
        MatrixBlock(const E &x) : x{x} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const{
            return x.elem(row_start+r, col_start+c);
        }

        static constexpr Index row = row_size;
        static constexpr Index col = col_size;

        inline Matrix<Type, row, col> eval() const{
            return Matrix<Type, row, col>(*this);
        }

        const E &x;
    };


    template <Index row_start, Index col_start, Index row_size, Index col_size, typename E>
        requires is_matrix_v<E> && is_between_v<0, row_start, E::row> && is_between_v<0, col_start, E::col> &&
                 is_between_v<0, row_start+row_size, E::row+1> && is_between_v<0, col_start+col_size, E::col+1>
    MatrixBlock<row_start, col_start, row_size, col_size, E> Block(const MatrixExpr<E> &x){
        return MatrixBlock<row_start, col_start, row_size, col_size, E>(static_cast<const E&>(x));
    }

}