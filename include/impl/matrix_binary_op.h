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
#include <stdexcept>
#include <type_traits>

// Peanut headers
#include "common.h"
#include "matrix_type_traits.h"

// Dependencies headers

namespace Peanut {

    template<typename T, Index Row, Index Col> requires std::is_arithmetic_v<T> && (Row > 0) && (Col > 0)
    struct Matrix;

    template <typename E1, typename E2> requires is_equal_size_mat_v<E1, E2>
    struct MatrixSum : public MatrixExpr<MatrixSum<E1, E2>>{
        using Type = typename E1::Type;
        MatrixSum(const E1 &x, const E2 &y) : x{x}, y{y} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const{
            return x.elem(r, c) + y.elem(r, c);
        }

        static constexpr Index row = E1::row;
        static constexpr Index col = E1::col;

        inline auto eval() const{
            return Matrix<Type, row, col>(*this);
        }

        const E1 &x;
        const E2 &y;
    };

    // =========================================================================

    template <typename E1, typename E2> requires is_equal_size_mat_v<E1, E2>
    struct MatrixSub : public MatrixExpr<MatrixSub<E1, E2>>{
        using Type = typename E1::Type;
        MatrixSub(const E1 &x, const E2 &y) : x{x}, y{y} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const{
            return x.elem(r, c) - y.elem(r, c);
        }

        static constexpr Index row = E1::row;
        static constexpr Index col = E1::col;

        inline auto eval() const{
            return Matrix<Type, row, col>(*this);
        }

        const E1 &x;
        const E2 &y;
    };

    // =========================================================================
    
    template <typename E, typename T> requires is_matrix_v<E> && std::is_arithmetic_v<T>
    struct MatrixMultScalar : public MatrixExpr<MatrixMultScalar<E, T>>{
        using Type = typename std::conditional<
                                    std::is_floating_point_v<typename E::Type> || std::is_floating_point_v<T>,
                                    Float, T>::type;
        MatrixMultScalar(const E &x, T y) : x{x}, y{y} {}

        // Static polymorphism implementation of MatrixExpr
        inline Type elem(Index r, Index c) const{
            return static_cast<Type>(x.elem(r, c)) * static_cast<Type>(y);
        }

        static constexpr Index row = E::row;
        static constexpr Index col = E::col;

        inline auto eval() const{
            return Matrix<Type, row, col>(*this);
        }

        const E &x;
        T y;
    };
    
    // =========================================================================

    template <typename E1, typename E2> requires (E1::col==E2::row)
    struct MatrixMult : public MatrixExpr<MatrixMult<E1, E2>>{
        using Type = typename E1::Type;
        MatrixMult(const E1 &_x, const E2 &_y) : x_eval{_x}, y_eval{_y} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const{
            auto ret = x_eval.elem(r, 0) * y_eval.elem(0, c);
            for(Index i=1;i<E1::col;i++){
                ret += x_eval.elem(r, i) * y_eval.elem(i, c);
            }
            return ret;
        }

        static constexpr Index row = E1::row;
        static constexpr Index col = E2::col;

        inline auto eval() const{
            return Matrix<Type, row, col>(*this);
        }

        // Specify member type as Matrix for evaluation
        const Matrix<Type, E1::row, E1::col> x_eval;
        const Matrix<Type, E2::row, E2::col> y_eval;
    };

    // =========================================================================
    
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

}

namespace Peanut{
    template <typename E1, typename E2> requires is_equal_size_mat_v<E1, E2>
    MatrixSum<E1, E2> operator+(const MatrixExpr<E1> &x, const MatrixExpr<E2> &y){
        return MatrixSum<E1, E2>(static_cast<const E1&>(x), static_cast<const E2&>(y));
    }

    // =========================================================================

    template <typename E1, typename E2> requires is_equal_size_mat_v<E1, E2>
    MatrixSub<E1, E2> operator-(const MatrixExpr<E1> &x, const MatrixExpr<E2> &y){
        return MatrixSub<E1, E2>(static_cast<const E1&>(x), static_cast<const E2&>(y));
    }

    // =========================================================================

    template <typename E, typename T> requires is_matrix_v<E> && std::is_arithmetic_v<T>
    MatrixMultScalar<E, T> operator*(const MatrixExpr<E> &x, const T &y){
        return MatrixMultScalar<E, T>(static_cast<const E&>(x), y);
    }

    template <typename E, typename T> requires is_matrix_v<E> && std::is_arithmetic_v<T>
    MatrixMultScalar<E, T> operator*(const T x, const MatrixExpr<E> &y){
        return MatrixMultScalar<E, T>(static_cast<const E&>(y), x);
    }

    // =========================================================================

    // General implementation
    template <typename E1, typename E2> requires (E1::col==E2::row)
    MatrixMult<E1, E2> operator*(const MatrixExpr<E1> &x, const MatrixExpr<E2> &y){
        return MatrixMult<E1, E2>(static_cast<const E1&>(x), static_cast<const E2&>(y));
    }

    // =========================================================================

    template <typename E, typename T> requires is_matrix_v<E> && std::is_arithmetic_v<T>
    MatrixDivScalar<E, T> operator/(const MatrixExpr<E> &x, const T &y){
        return MatrixDivScalar<E, T>(static_cast<const E&>(x), y);
    }
}