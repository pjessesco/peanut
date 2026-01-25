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
     * @tparam E Matrix expression type operand.
     * @tparam T Scalar type operand.
     */
    template<typename E, typename T>
        requires is_matrix_v<E> && std::is_arithmetic_v<T>
    struct MatrixMultScalar : public MatrixExpr<MatrixMultScalar<E, T>> {
        using Type = typename std::conditional<
                std::is_floating_point_v<typename E::Type> || std::is_floating_point_v<T>,
                Float, T>::type;
        MatrixMultScalar(const E &x, T y) : x{x}, y{y} {}

        // Static polymorphism implementation of MatrixExpr
        INLINE Type operator()(Index r, Index c) const {
            return static_cast<Type>(x(r, c)) * static_cast<Type>(y);
        }

        static constexpr Index Row = E::Row;
        static constexpr Index Col = E::Col;

        INLINE void eval(Matrix<Type, Row, Col> &_result) const {
            for (int i=0;i<Row;i++) {
                for (int j=0;j<Col;j++) {
                    _result(i,j) = static_cast<Type>(x(i, j)) * static_cast<Type>(y);
                }
            }
        }

        const E &x;
        T y;
    };
}

namespace Peanut {

    /**
     * @brief Scalar multiplication for small matrices (eager evaluation)
     */
    template<typename E, typename T>
        requires is_matrix_v<E> && std::is_arithmetic_v<T> && is_small_matrix_v<E>
    Matrix<typename Impl::MatrixMultScalar<E, T>::Type, E::Row, E::Col> operator*(const MatrixExpr<E> &x, const T &y) {
        using ResultType = typename Impl::MatrixMultScalar<E, T>::Type;
        Matrix<typename E::Type, E::Row, E::Col> x_eval = static_cast<const E&>(x);
        Matrix<ResultType, E::Row, E::Col> result;
        for (Index i = 0; i < E::Row * E::Col; i++) {
            result.m_data[i] = static_cast<ResultType>(x_eval.m_data[i]) * static_cast<ResultType>(y);
        }
        return result;
    }

    /**
     * @brief Scalar multiplication for large matrices (lazy evaluation)
     */
    template<typename E, typename T>
        requires is_matrix_v<E> && std::is_arithmetic_v<T> && (!is_small_matrix_v<E>)
    Impl::MatrixMultScalar<E, T> operator*(const MatrixExpr<E> &x, const T &y) {
        return Impl::MatrixMultScalar<E, T>(static_cast<const E &>(x), y);
    }

    /**
     * @brief Scalar multiplication for small matrices (eager evaluation, reversed operands)
     */
    template<typename E, typename T>
        requires is_matrix_v<E> && std::is_arithmetic_v<T> && is_small_matrix_v<E>
    Matrix<typename Impl::MatrixMultScalar<E, T>::Type, E::Row, E::Col> operator*(const T x, const MatrixExpr<E> &y) {
        using ResultType = typename Impl::MatrixMultScalar<E, T>::Type;
        Matrix<typename E::Type, E::Row, E::Col> y_eval = static_cast<const E&>(y);
        Matrix<ResultType, E::Row, E::Col> result;
        for (Index i = 0; i < E::Row * E::Col; i++) {
            result.m_data[i] = static_cast<ResultType>(x) * static_cast<ResultType>(y_eval.m_data[i]);
        }
        return result;
    }

    /**
     * @brief Scalar multiplication for large matrices (lazy evaluation, reversed operands)
     */
    template<typename E, typename T>
        requires is_matrix_v<E> && std::is_arithmetic_v<T> && (!is_small_matrix_v<E>)
    Impl::MatrixMultScalar<E, T> operator*(const T x, const MatrixExpr<E> &y) {
        return Impl::MatrixMultScalar<E, T>(static_cast<const E &>(y), x);
    }
}
