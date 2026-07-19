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
     * @brief Expression class which represents `operator*()`.
     * @details Note that `MatrixMult` evaluates its operands internally
     *          for the performance issues.
     * @tparam E1 Left hand side matrix expression type.
     * @tparam E2 Right hand side matrix expression type.
     */
    template<typename E1, typename E2>
        requires(E1::Col == E2::Row)
    struct MatrixMult : public MatrixExpr<MatrixMult<E1, E2>> {
        using Type = typename E1::Type;

        // If an operand is already an evaluated `Matrix`, hold a reference to it
        // instead of copying. Otherwise evaluate the expression into a Matrix.
        static constexpr bool is_x_mat = std::is_same_v<E1, Matrix<Type, E1::Row, E1::Col>>;
        static constexpr bool is_y_mat = std::is_same_v<E2, Matrix<Type, E2::Row, E2::Col>>;

        using XEval = std::conditional_t<is_x_mat, const E1 &, Matrix<Type, E1::Row, E1::Col>>;
        using YEval = std::conditional_t<is_y_mat, const E2 &, Matrix<Type, E2::Row, E2::Col>>;

        MatrixMult(const E1 &_x, const E2 &_y) : x_eval{eval_operand<is_x_mat>(_x)},
                                                 y_eval{eval_operand<is_y_mat>(_y)} {}

        // Static polymorphism implementation of MatrixExpr
        INLINE auto operator()(Index r, Index c) const {
            auto ret = x_eval(r, 0) * y_eval(0, c);
            for (Index i = 1; i < E1::Col; i++) {
                ret += x_eval(r, i) * y_eval(i, c);
            }
            return ret;
        }

        static constexpr Index Row = E1::Row;
        static constexpr Index Col = E2::Col;

        INLINE void eval(Matrix<Type, Row, Col> &_result) const {
            for (int i=0;i<Row;i++) {
                for (int j=0;j<Col;j++) {
                    _result(i, j) = x_eval(i, 0) * y_eval(0, j);
                }
                for (Index k = 1; k < E1::Col; k++) {
                    for (int j=0;j<Col;j++) {
                        _result(i, j) += x_eval(i, k) * y_eval(k, j);
                    }
                }
            }
        }

        // Bind a reference for `Matrix` operands, evaluate into a temporary otherwise.
        template<bool IsMat, typename E>
        static decltype(auto) eval_operand(const E &_e) {
            if constexpr (IsMat) {
                return (_e);
            } else {
                Matrix<Type, E::Row, E::Col> ret;
                _e.eval(ret);
                return ret;
            }
        }

        // Reference to a `Matrix` operand, or an evaluated Matrix for expressions.
        XEval x_eval;
        YEval y_eval;
    };

}

namespace Peanut {

    /**
     * @brief Multiplication between matrices. See `Impl::MatrixMult`.
     * @tparam E1 Left hand side matrix expression type.
     * @tparam E2 Right hand side matrix expression type.
     * @return Constructed `Impl::MatrixMult` instance
     */
    template<typename E1, typename E2>
        requires(E1::Col == E2::Row)
    Impl::MatrixMult<E1, E2> operator*(const MatrixExpr<E1> &x, const MatrixExpr<E2> &y) {
        return Impl::MatrixMult<E1, E2>(static_cast<const E1 &>(x), static_cast<const E2 &>(y));
    }
}
