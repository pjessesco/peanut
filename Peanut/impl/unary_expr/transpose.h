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

    template<typename E>
        requires is_matrix_v<E>
    struct MatrixTranspose : public MatrixExpr<MatrixTranspose<E>> {
        using Type = typename E::Type;
        MatrixTranspose(const E &x) : x{x} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const {
            return x.elem(c, r);
        }

        static constexpr Index row = E::col;
        static constexpr Index col = E::row;

        inline Matrix<Type, row, col> eval() const {
            return Matrix<Type, row, col>(*this);
        }

        const E &x;
    };
}

namespace Peanut{
    template<typename E>
        requires is_matrix_v<E>
    E T(const Impl::MatrixTranspose<E> &x) {
        return static_cast<const E &>(x.x);
    }

    template<typename E>
        requires is_matrix_v<E>
    Impl::MatrixTranspose<E> T(const MatrixExpr<E> &x) {
        return Impl::MatrixTranspose<E>(static_cast<const E &>(x));
    }
}