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
     * @brief Expression class which represents an inverse matrix.
     * @details Note that `MatrixInverse` evaluates its input expression
     *          internally during construction to avoid duplicated calculation.
     * @tparam E Matrix expression type.
     */
    // TODO : This struct can be deleted, since the inverse of matrix is equivalent
    //        to the transpose of the cofactor (i.e., Inv(x) == T(Cofactor(x))).
    template<typename E>
        requires is_matrix_v<E> && is_square_v<E>
    struct MatrixInverse : public MatrixExpr<MatrixInverse<E>> {
        using Type = Float;
        MatrixInverse(const E &_x) : x{_x} {
            // Evaluate adjugate matrix
            for_<row>([&](auto r) {
                for_<col>([&](auto c) {
                    constexpr Index rv = r.value;
                    constexpr Index cv = c.value;
                    const Float e = static_cast<Float>(SubMat<rv, cv>(_x).eval().det());
                    if constexpr ((rv + cv) % 2 == 0) {
                        mat_eval.elem(cv, rv) = e;
                    } else {
                        mat_eval.elem(cv, rv) = -e;
                    }
                });
            });
            invdet = static_cast<Float>(1) / _x.eval().det();
        }

        // Static polymorphism implementation of MatrixExpr
        inline Float elem(Index r, Index c) const {
            return invdet * mat_eval.elem(r, c);
        }

        static constexpr Index row = E::row;
        static constexpr Index col = E::col;

        inline Matrix<Type, row, col> eval() const {
            return Matrix<Type, row, col>(*this);
        }

        const E &x;// used for optimization
        Matrix<Float, row, col> mat_eval;
        Float invdet;
    };
}

namespace Peanut {
    /**
     * @brief Inverse operation of matrix. See `Impl::MatrixInverse`
     *        and https://en.wikipedia.org/wiki/Invertible_matrix for details.
     * @tparam E Matrix expression type.
     * @return Constructed `Impl::MatrixInverse` instance
     */
    template<typename E>
        requires is_matrix_v<E> && is_square_v<E>
    Impl::MatrixInverse<E> Inverse(const MatrixExpr<E> &x) {
        return Impl::MatrixInverse<E>(static_cast<const E &>(x));
    }

    /**
     * @brief Template specialization of `Inverse()` which represents an
     *        inverse of an inverse of a matrix. It is equivalent to
     *        a input of the given parameter (i.e., Inv(Inv(x)) = x), so
     *        it does not construct a `MatrixInverse` instance.
     * @tparam E Matrix expression type.
     * @param x `MatrixInverse<E>` type matrix expression.
     * @return Input of the given parameter `x`
     */
    template<typename E>
        requires is_matrix_v<E> && is_square_v<E>
    E Inverse(const Impl::MatrixInverse<E> &x) {
        return static_cast<const E &>(x.x);
    }
}