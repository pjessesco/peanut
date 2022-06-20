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

    template <typename E1, typename E2> requires is_equal_size_mat_v<E1, E2>
    struct MatrixSum : public MatrixExpr<MatrixSum<E1, E2>>{
        MatrixSum(const E1 &x, const E2 &y) : x{x}, y{y} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const{
            return x.elem(r, c) + y.elem(r, c);
        }

        [[nodiscard]] static constexpr Index row() {return E1::row();}
        [[nodiscard]] static constexpr Index col() {return E1::col();}

        const E1 &x;
        const E2 &y;
    };

    template <typename E1, typename E2> requires is_equal_size_mat_v<E1, E2>
    MatrixSum<E1, E2> operator+(const MatrixExpr<E1> &x, const MatrixExpr<E2> &y){
        return MatrixSum<E1, E2>(static_cast<const E1&>(x), static_cast<const E2&>(y));
    }

    // =========================================================================

    template <typename E1, typename E2> requires is_equal_size_mat_v<E1, E2>
    struct MatrixSub : public MatrixExpr<MatrixSub<E1, E2>>{
        MatrixSub(const E1 &x, const E2 &y) : x{x}, y{y} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const{
            return x.elem(r, c) - y.elem(r, c);
        }

        [[nodiscard]] static constexpr Index row() {return E1::row();}
        [[nodiscard]] static constexpr Index col() {return E1::col();}

        const E1 &x;
        const E2 &y;
    };

    template <typename E1, typename E2> requires is_equal_size_mat_v<E1, E2>
    MatrixSub<E1, E2> operator-(const MatrixExpr<E1> &x, const MatrixExpr<E2> &y){
        return MatrixSub<E1, E2>(static_cast<const E1&>(x), static_cast<const E2&>(y));
    }

    // =========================================================================

    template <typename E1, typename E2> requires (E1::col()==E2::row())
    struct MatrixMult : public MatrixExpr<MatrixMult<E1, E2>>{
        MatrixMult(const E1 &x, const E2 &y) : x{x}, y{y} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const{
            auto ret = x.elem(r, 0) * y.elem(0, c);
            for(Index i=1;i<E1::col();i++){
                ret += x.elem(r, i) * y.elem(i, c);
            }
            return ret;
        }
        [[nodiscard]] static constexpr Index row() {return E1::row();}
        [[nodiscard]] static constexpr Index col() {return E2::col();}

        const E1 &x;
        const E2 &y;
    };

    // General implementation
    template <typename E1, typename E2> requires (E1::col()==E2::row())
    MatrixMult<E1, E2> operator*(const MatrixExpr<E1> &x, const MatrixExpr<E2> &y){
        return MatrixMult<E1, E2>(static_cast<const E1&>(x), static_cast<const E2&>(y));
    }


}