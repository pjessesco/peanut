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
#include <algorithm>
#include <array>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <type_traits>

// Peanut headers
#include "common.h"
#include "matrix_type_traits.h"

// Dependencies headers

namespace Peanut {

    template<typename T, Index Row, Index Col> requires std::is_arithmetic_v<T> && (Row > 0) && (Col > 0)
    struct Matrix;

    template <typename E> requires is_matrix_v<E>
    struct MatrixTranspose : public MatrixExpr<MatrixTranspose<E>>{
        using Type = typename E::Type;
        MatrixTranspose(const E &x) : x{x} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const{
            return x.elem(c, r);
        }
        [[nodiscard]] static constexpr Index row() {return E::col();}
        [[nodiscard]] static constexpr Index col() {return E::row();}

        const E &x;
    };

    template <typename E> requires is_matrix_v<E>
    E T(const MatrixTranspose<E> &x){
        return static_cast<const E&>(x.x);
    }

    template <typename E> requires is_matrix_v<E>
    MatrixTranspose<E> T(const MatrixExpr<E> &x){
        return MatrixTranspose<E>(static_cast<const E&>(x));
    }

    // =========================================================================

    template <Index row_start, Index col_start, Index row_size, Index col_size, typename E>
    requires is_matrix_v<E> && is_between_v<0, row_start, E::row()> && is_between_v<0, col_start, E::col()> &&
             is_between_v<0, row_start+row_size, E::row()> && is_between_v<0, col_start+col_size, E::col()>
    struct MatrixBlock : public MatrixExpr<MatrixBlock<row_start, col_start, row_size, col_size, E>>{
        using Type = typename E::Type;
        MatrixBlock(const E &x) : x{x} {}
        
        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const{
            return x.elem(row_start+r, col_start+c);
        }
        [[nodiscard]] static constexpr Index row() {return row_size;}
        [[nodiscard]] static constexpr Index col() {return col_size;}

        const E &x;
    };

    template <Index row_start, Index col_start, Index row_size, Index col_size, typename E>
    requires is_matrix_v<E> && is_between_v<0, row_start, E::row()> && is_between_v<0, col_start, E::col()> &&
             is_between_v<0, row_start+row_size, E::row()> && is_between_v<0, col_start+col_size, E::col()>
    MatrixBlock<row_start, col_start, row_size, col_size, E> Block(const MatrixExpr<E> &x){
        return MatrixBlock<row_start, col_start, row_size, col_size, E>(static_cast<const E&>(x));
    }

    // =========================================================================

    template <Index row_ex, Index col_ex, typename E>
    requires is_matrix_v<E> && is_square_v<E> && is_between_v<0, row_ex, E::row()> && is_between_v<0, col_ex, E::col()>
    struct MatrixAdjugate : public MatrixExpr<MatrixAdjugate<row_ex, col_ex, E>>{
        using Type = typename E::Type;
        MatrixAdjugate(const E &x) : x{x} {}
        
        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const{
            return x.elem(r<row_ex?r:r+1, c<col_ex?c:c+1);
        }
        [[nodiscard]] static constexpr Index row() {return E::row() - 1;}
        [[nodiscard]] static constexpr Index col() {return E::col() - 1;}
        
        const E &x;
    };

    template <Index row_ex, Index col_ex, typename E>
    MatrixAdjugate<row_ex, col_ex, E> Adj(const MatrixExpr<E> &x){
        return MatrixAdjugate<row_ex, col_ex, E>(static_cast<const E&>(x));
    }

    // =========================================================================

    template <typename T, typename E> requires std::is_arithmetic_v<T> && is_matrix_v<E>
    struct MatrixCastType : public MatrixExpr<MatrixCastType<T, E>>{
        using Type = T;
        MatrixCastType(const E &x) : x{x} {}

        // Static polymorphism implementation of MatrixExpr
        inline T elem(Index r, Index c) const{
            return static_cast<T>(x.elem(r, c));
        }
        [[nodiscard]] static constexpr Index row() {return E::row();}
        [[nodiscard]] static constexpr Index col() {return E::col();}

        const E &x;
    };

    template <typename T, typename E> requires std::is_arithmetic_v<T> && is_matrix_v<E>
    MatrixCastType<T, E> cast_to(const MatrixExpr<E> &x){
        return MatrixCastType<T, E>(static_cast<const E&>(x));
    }

    // =========================================================================

}