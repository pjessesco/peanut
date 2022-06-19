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
#include <initializer_list>
#include <type_traits>
#include <array>
#include <algorithm>
#include <cstring>

// Peanut headers
#include "common.h"

// Dependencies headers

namespace Peanut {

    template <typename E>
    struct MatrixExpr{
        auto operator[] (Index i) const{
            return static_cast<const E&>(*this)[i];
        }
        [[nodiscard]] static constexpr Index row() {
            return E::row();
        }
        [[nodiscard]] static constexpr Index col() {
            return E::col();
        }
    };

    template<typename T, Index Row, Index Col>
    requires std::is_arithmetic_v<T> && (Row > 0) && (Col > 0)
    struct Matrix : public MatrixExpr<Matrix<T, Row, Col>>{
        Matrix() {m_data.fill(t_0);}
        Matrix(const std::initializer_list<T> &il) {
            std::copy(il.begin(), il.end(), m_data);
        }
        explicit Matrix(const std::array<T, Row * Col> &data) : m_data{data} {}
        explicit Matrix(const std::vector<T> &data) : m_data{data} {}

        template<typename E>
        Matrix(const MatrixExpr<E> &expr) {
            for(int i=0;i<Row*Col;i++){
                m_data[i] = expr[i];
            }
        }

        static Matrix zeros() {return Matrix();}

        static Matrix identity() requires(Row == Col) {
            Matrix a;
            a.m_data.fill(t_0);
            for (Index i = 0; i < Row * Col; i += (Row + 1)) {
                a.m_data[i] = t_1;
            }
            return a;
        }

        // Static polymorphism implementation of MatrixExpr
        T operator[](Index i) const {return m_data[i];}
        T& operator[](Index i) {return m_data[i];}

        [[nodiscard]] static constexpr Index row() {return Row;}
        [[nodiscard]] static constexpr Index col() {return Col;}

        void print_mat() const {
            for (int i = 0; i < Row * Col; i++) {
                std::cout << m_data[i] << " ";
            }
        }

    private:
        static constexpr T t_1 = static_cast<T>(1);
        static constexpr T t_0 = static_cast<T>(0);
        std::array<T, Row*Col> m_data;
    };
}