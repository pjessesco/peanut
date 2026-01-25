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
        MatrixMult(const E1 &_x, const E2 &_y) {
            _x.eval(x_eval);
            _y.eval(y_eval);
        }

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
        static constexpr Index K = E1::Col;

        // L1 cache size (conservative estimate: 32KB)
        static constexpr std::size_t L1_CACHE_SIZE = 32768;
        // Working set: A(Row×K) + B(K×Col) + C(Row×Col)
        static constexpr std::size_t working_set_size =
            (Row * K + K * Col + Row * Col) * sizeof(Type);
        // Use cache-friendly i-k-j loop when:
        // 1. Working set exceeds L1 cache, OR
        // 2. K > 32 (column access in B matrix benefits from row-major traversal)
        static constexpr bool use_cache_friendly_loop =
            (working_set_size > L1_CACHE_SIZE) || (K > 32);
        // Always use eval() for MatrixMult (operands already evaluated)
        static constexpr bool prefers_eval = true;

        INLINE void eval(Matrix<Type, Row, Col> &_result) const {
            if constexpr (use_cache_friendly_loop) {
                // Large matrices: i-k-j order for cache efficiency
                for (Index i = 0; i < Row * Col; i++) {
                    _result.m_data[i] = Type{0};
                }
                for (Index i = 0; i < Row; i++) {
                    for (Index k = 0; k < K; k++) {
                        const Type x_ik = x_eval.m_data[i * K + k];
                        for (Index j = 0; j < Col; j++) {
                            _result.m_data[i * Col + j] += x_ik * y_eval.m_data[k * Col + j];
                        }
                    }
                }
            }
            else {
                // Small matrices: i-j-k order (simpler, less overhead)
                for (Index i = 0; i < Row; i++) {
                    for (Index j = 0; j < Col; j++) {
                        Type sum = x_eval.m_data[i * K] * y_eval.m_data[j];
                        for (Index k = 1; k < K; k++) {
                            sum += x_eval.m_data[i * K + k] * y_eval.m_data[k * Col + j];
                        }
                        _result.m_data[i * Col + j] = sum;
                    }
                }
            }
        }

        // Specify member type as Matrix for evaluation
        Matrix<Type, E1::Row, E1::Col> x_eval;
        Matrix<Type, E2::Row, E2::Col> y_eval;
    };

}

namespace Peanut {

    /**
     * @brief Multiplication for small matrices (eager evaluation)
     */
    template<typename E1, typename E2>
        requires(E1::Col == E2::Row) && (E1::Row * E2::Col <= EAGER_EVAL_THRESHOLD)
    Matrix<typename E1::Type, E1::Row, E2::Col> operator*(const MatrixExpr<E1> &x, const MatrixExpr<E2> &y) {
        using Type = typename E1::Type;
        constexpr Index Row = E1::Row;
        constexpr Index Col = E2::Col;
        constexpr Index K = E1::Col;

        Matrix<Type, E1::Row, E1::Col> x_eval = static_cast<const E1&>(x);
        Matrix<Type, E2::Row, E2::Col> y_eval = static_cast<const E2&>(y);
        Matrix<Type, Row, Col> result;

        for (Index i = 0; i < Row; i++) {
            for (Index j = 0; j < Col; j++) {
                Type sum = x_eval.m_data[i * K] * y_eval.m_data[j];
                for (Index k = 1; k < K; k++) {
                    sum += x_eval.m_data[i * K + k] * y_eval.m_data[k * Col + j];
                }
                result.m_data[i * Col + j] = sum;
            }
        }
        return result;
    }

    /**
     * @brief Multiplication for large matrices (lazy evaluation)
     */
    template<typename E1, typename E2>
        requires(E1::Col == E2::Row) && (E1::Row * E2::Col > EAGER_EVAL_THRESHOLD)
    Impl::MatrixMult<E1, E2> operator*(const MatrixExpr<E1> &x, const MatrixExpr<E2> &y) {
        return Impl::MatrixMult<E1, E2>(static_cast<const E1 &>(x), static_cast<const E2 &>(y));
    }

}
