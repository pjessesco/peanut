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
     * @brief Expression class which represents a matrix type casting.
     * @tparam T Target data type
     * @tparam E Matrix expression type.
     */
    template<typename T, typename E>
        requires std::is_arithmetic_v<T> && is_matrix_v<E>
    struct MatrixCastType : public MatrixExpr<MatrixCastType<T, E>> {
        using Type = T;
        MatrixCastType(const E &x) : x{x} {}

        // Static polymorphism implementation of MatrixExpr
        INLINE T elem(Index r, Index c) const {
            return static_cast<T>(x.elem(r, c));
        }

        static constexpr Index Row = E::Row;
        static constexpr Index Col = E::Col;

        INLINE Matrix<Type, Row, Col> eval() const {
            return Matrix<Type, Row, Col>(*this);
        }

        const E &x;
    };
}

namespace Peanut {
    /**
     * @brief Type casting of a matrix expression.
     * @tparam T Target data type.
     * @tparam E Matrix expression type.
     * @return Constructed `Impl::MatrixCastType` instance.
     *
     *     Matrix<int, 2, 2> mat{1,2,
     *                           3,4};
     *
     *     Matrix<float, 2, 2> ev = Cast<float>(mat);
     *
     */
    template<typename T, typename E>
        requires std::is_arithmetic_v<T> && is_matrix_v<E>
    Impl::MatrixCastType<T, E> Cast(const MatrixExpr<E> &x) {
        return Impl::MatrixCastType<T, E>(static_cast<const E &>(x));
    }
}
