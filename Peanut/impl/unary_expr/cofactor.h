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

    /**
     * @brief Expression class which represents a cofactor matrix.
     * @details Note that `MatrixCofactor` evaluates its input expression
     *          internally during construction to avoid duplicated calculation.
     * @tparam E Matrix expression type.
     */
    template<typename E>
        requires is_matrix_v<E> && is_square_v<E>
    struct MatrixCofactor : public MatrixExpr<MatrixCofactor<E>> {
        using Type = typename E::Type;
        MatrixCofactor(const E &_x) {
            for_<row>([&](auto r) {
                for_<col>([&](auto c) {
                    constexpr Index rv = r.value;
                    constexpr Index cv = c.value;
                    const Type e = SubMat<rv, cv>(_x).eval().det();
                    if constexpr ((rv + cv) % 2 == 0) {
                        mat_eval.elem(rv, cv) = e;
                    } else {
                        mat_eval.elem(rv, cv) = -e;
                    }
                });
            });
        }

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const {
            return mat_eval.elem(r, c);
        }

        static constexpr Index row = E::row;
        static constexpr Index col = E::col;

        inline Matrix<Type, row, col> eval() const {
            return mat_eval;
        }

        Peanut::Matrix<Type, row, col> mat_eval;
    };
}

namespace Peanut{
    /**
     * @brief Cofactor operation (i.e., signed minor) of a matrix.
     *        See `Impl::MatrixCofactor` and https://en.wikipedia.org/wiki/Cofactor_(linear_algebra)
     * @tparam E Matrix expression type.
     * @return Constructed `Impl::MatrixCofactor` instance
     */
    template <typename E> requires is_matrix_v<E> && is_square_v<E>
    Impl::MatrixCofactor<E> Cofactor(const MatrixExpr<E> &x){
        return Impl::MatrixCofactor<E>(static_cast<const E&>(x));
    }
}