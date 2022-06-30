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
#include <stdexcept>

// Peanut headers
#include "impl/common.h"
#include "impl/matrix_type_traits.h"

// Dependencies headers

namespace Peanut{

    template<typename T, Index Row, Index Col> requires std::is_arithmetic_v<T> && (Row > 0) && (Col > 0)
    struct Matrix;

    template <typename E, typename T> requires is_matrix_v<E> && std::is_arithmetic_v<T>
    struct MatrixDivScalar : public MatrixExpr<MatrixDivScalar<E, T>>{
        using Type = Float;
        MatrixDivScalar(const E &x, T y) : x{x}, y{y} {
            if(is_zero(y)){
                throw std::invalid_argument("Divide by zero");
            }
        }

        // Static polymorphism implementation of MatrixExpr
        inline Float elem(Index r, Index c) const{
            return static_cast<Type>(x.elem(r, c)) / static_cast<Float>(y);
        }

        static constexpr Index row = E::row;
        static constexpr Index col = E::col;

        inline auto eval() const{
            return Matrix<Type, row, col>(*this);
        }

        const E &x;
        T y;
    };

    template <typename E, typename T> requires is_matrix_v<E> && std::is_arithmetic_v<T>
    MatrixDivScalar<E, T> operator/(const MatrixExpr<E> &x, const T &y){
        return MatrixDivScalar<E, T>(static_cast<const E&>(x), y);
    }

}