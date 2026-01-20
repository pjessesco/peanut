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

#include <array>
#include <type_traits>

template <typename T, size_t Row, size_t Col>
struct MatrixBT{
    MatrixBT() {m_data.fill(t_0);}

    template <typename ...TList>
        MatrixBT(TList ... tlist) : m_data{std::forward<T>(tlist)...} {}

    MatrixBT operator+(const MatrixBT &o) const{
        MatrixBT tmp(*this);
        for (size_t i = 0; i < Row*Col; ++i) {
            tmp.m_data[i] += o.m_data[i];
        }
        return tmp;
    }

    MatrixBT operator-(const MatrixBT &o) const{
        MatrixBT tmp(*this);
        for (size_t i = 0; i < Row*Col; ++i) {
            tmp.m_data[i] -= o.m_data[i];
        }
        return tmp;
    }

    template <size_t Col2>
    MatrixBT<T, Row, Col2> operator*(const MatrixBT<T, Col, Col2> &o) const{
        MatrixBT<T, Row, Col2> tmp;
        for (size_t i = 0; i < Row; ++i) {
            for (size_t j = 0; j < Col2; ++j) {
                T sum = t_0;
                for (size_t k = 0; k < Col; ++k) {
                    sum += m_data[i * Col + k] * o.m_data[k * Col2 + j];
                }
                tmp.m_data[i * Col2 + j] = sum;
            }
        }
        return tmp;
    }

    MatrixBT<T, Col, Row> transpose() const {
        MatrixBT<T, Col, Row> tmp;
        for (size_t i = 0; i < Row; ++i) {
            for (size_t j = 0; j < Col; ++j) {
                tmp.m_data[j * Row + i] = m_data[i * Col + j];
            }
        }
        return tmp;
    }

    T& operator()(size_t r, size_t c) { return m_data[r * Col + c]; }
    T operator()(size_t r, size_t c) const { return m_data[r * Col + c]; }

    static constexpr T t_1 = static_cast<T>(1);
    static constexpr T t_0 = static_cast<T>(0);
    std::array<T, Row*Col> m_data;
};
