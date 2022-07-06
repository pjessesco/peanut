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

namespace Peanut::Impl {
    template<typename E, typename T>
        requires is_matrix_v<E> && std::is_arithmetic_v<T>
    struct MatrixMultScalar : public MatrixExpr<MatrixMultScalar<E, T>> {
        using Type = typename std::conditional<
                std::is_floating_point_v<typename E::Type> || std::is_floating_point_v<T>,
                Float, T>::type;
        MatrixMultScalar(const E &x, T y) : x{x}, y{y} {}

        // Static polymorphism implementation of MatrixExpr
        inline Type elem(Index r, Index c) const {
            return static_cast<Type>(x.elem(r, c)) * static_cast<Type>(y);
        }

        static constexpr Index row = E::row;
        static constexpr Index col = E::col;

        inline auto eval() const {
            return Matrix<Type, row, col>(*this);
        }

        const E &x;
        T y;
    };
}

namespace Peanut {
    template<typename E, typename T>
        requires is_matrix_v<E> && std::is_arithmetic_v<T>
    Impl::MatrixMultScalar<E, T> operator*(const MatrixExpr<E> &x, const T &y) {
        return Impl::MatrixMultScalar<E, T>(static_cast<const E &>(x), y);
    }

    template<typename E, typename T>
        requires is_matrix_v<E> && std::is_arithmetic_v<T>
    Impl::MatrixMultScalar<E, T> operator*(const T x, const MatrixExpr<E> &y) {
        return Impl::MatrixMultScalar<E, T>(static_cast<const E &>(y), x);
    }
}