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
#include "matrix.h"
#include "matrix_type_traits.h"

// Dependencies headers

namespace Peanut {

    template <typename E> requires is_matrix_v<E>
    struct MatrixTranspose : public MatrixExpr<MatrixTranspose<E>>{
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

    template <typename E> requires is_matrix_v<E> && is_square_v<E>
    struct MatrixInverse : public MatrixExpr<MatrixInverse<E>>{
        MatrixInverse(const E &x) : x{x} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const{
            return 0;
        }

        const E &x;
    };


    template <typename E> requires is_matrix_v<E> && is_square_v<E>
    E Inv(const MatrixInverse<E> &x){
        return static_cast<const E&>(x.x);
    }

    template <typename E> requires is_matrix_v<E> && is_square_v<E>
    MatrixInverse<E> Inv(const MatrixExpr<E> &x){
        return MatrixInverse<E>(static_cast<const E&>(x));
    }

    // =========================================================================

}