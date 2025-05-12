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
#include <array>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <type_traits>
#include <cmath>
#include <cstring>
#include <vector>

// Peanut headers
#include <Peanut/impl/common.h>
#include <Peanut/impl/matrix_type_traits.h>

// Dependencies headers

namespace Peanut {

    /**
     * @brief Base class of every matrix expressions in Peanut.
     * @details Peanut implements matrix expression by following CRTP concept
     *          to achieve a static polymorphism. Every derived class should
     *          follow its interface even they are not forced by the compiler
     *          (i.e., pure virtual method). Also they MUST provide their data
     *          type via `using Type = XXX`, and implement `eval()` method for
     *          matrix evaluation. See `Matrix<T, R, C>::eval()` for more
     *          explanation.
     * @tparam E Derived class. `MatrixExpr`
     */
    template <typename E>
    struct MatrixExpr{

        /**
         * @brief Get element in \p r 'th Row and \p c 'th column.
         * @details It is also used for evaluation of an arbitrary matrix expression
         *          by recursively calling in derived classes. See also
         *          `Matrix::Matrix(const MatrixExpr<E> &expr)` and
         *          `Matrix::operator(Index r, Index c)` and other derived classes.
         * @param r R index.
         * @param c C index.
         * @return Rvalue of an element in \p r 'th Row and \p c 'th column.
         */
        INLINE auto operator()(Index r, Index c) const{
            return static_cast<const E&>(*this)(r, c);
        }
    };

    /**
     * @brief Basic matrix class.
     * @tparam T Data type.
     * @tparam R Row size.
     * @tparam C Column size.
     */
    template<typename T, Index R, Index C> requires IS_ARITHMETIC_V<T> && (R > 0) && (C > 0)
    struct Matrix : public MatrixExpr<Matrix<T, R, C>>{

        /**
         * @brief Data type. See the detailed description of \p MatrixExpr.
         */
        using Type = T;

        /**
         * @brief Row size of the matrix.
         */
        static constexpr Index Row = R;

        /**
         * @brief Column size of the matrix.
         */
        static constexpr Index Col = C;

        /**
         * @brief Constructor without any initialization
         */
        Matrix() {}

        /**
         * @brief Constructor with row-major elements.
         * @param tlist Parameter pack with \p T types.
         *
         *     // {} is also available
         *     Matrix<int, 4, 4> mat1(1,2,3,4,
         *                            5,6,7,8,
         *                            9,0,1,2,
         *                            3,4,5,6);
         *
         */
        template <typename ...TList>
            requires (sizeof...(TList) == Row*Col)
        Matrix(TList ... tlist) : m_data{std::forward<T>(tlist)...} {}

        /**
         * @brief Constructor with std::array.
         * @param data A std::array having `T` type and \p R * \p C size.
         */
        explicit Matrix(const std::array<T, R * C> &data) {
            memcpy(m_data.data(), data.data(), sizeof(T)*R*C);
        }

        /**
         * @brief Constructor with std::vector.
         * @param data std::vector having `T` type.
         */
        explicit Matrix(const std::vector<T> &data) : m_data{data} {}

        /**
         * @brief Constructor from arbitrary Peanut matrix expression.
         * @details Lazy evaluation is performed when the given expression is
         *          substituted to other `Matrix`, or `Matrix::eval()` is called.
         * @param expr Arbitrary Peanut matrix expression.
         */
        template<typename E>
        Matrix(const MatrixExpr<E> &expr) requires is_equal_type_size_v<E, Matrix>{
            for(Index r=0;r< R;r++){
                for(Index c=0;c< C;c++){
                    m_data[r*C+c] = expr(r, c);
                }
            }
        }

        /**
         * @brief Factory function for zero matrix
         * @return Zero matrix with given \p R and \p C .
         */
        static Matrix zeros() {
            auto m = Matrix();
            memset(m.m_data.data(), 0, sizeof(T)*R*C);
            return m;
        }

        /**
         * @brief Construct identity matrix. Available only for square matrix case.
         * @return Identity matrix with given \p R and \p C .
         */
        static Matrix identity() requires is_square_v<Matrix> {
            Matrix a;
            memset(a.m_data.data(), 0, sizeof(T)*R*C);
            for (Index i = 0; i < R; i++) {
                a.m_data[i*C+i] = t_1;
            }
            return a;
        }

        /**
         * @brief Construct a matrix using given rows (i.e., `Matrix<Type, 1, Col>`)
         * @param tlist Parameter pack with `Matrix<Type, 1, Col>` types.
         * @return Constructed matrix
         *
         *     Peanut::Matrix<int, 1, 4> a{1,2,3,4};
         *     Peanut::Matrix<int, 1, 4> b{5,6,7,8};
         *     Peanut::Matrix<int, 1, 4> c{9,10,11,12};
         *
         *     auto mat = Peanut::Matrix<int, 3, 4>::from_rows(a, b, c);
         *     // 1  2  3  4
         *     // 5  6  7  8
         *     // 9 10 11 12
         *
         */
        template <typename ...RList>
            requires std::conjunction_v<std::is_same<Matrix<Type, 1, Col>, RList>...> &&
                     (sizeof...(RList) == Row)
        static Matrix from_rows(RList ... rlist){
            Matrix ret;
            int idx = 0;
            constexpr size_t copy_byte = sizeof(Type) * Col;
            for(const Matrix<Type, 1, Col> p : {rlist...}){
                memcpy(&(ret.m_data[idx*C]), p.m_data.data(), copy_byte);
                idx++;
            }
            return ret;
        }

        /**
         * @brief Construct a matrix using given columns (i.e., `Matrix<Type, Row, 1>`)
         * @param tlist Parameter pack with `Matrix<Type, Row, 1>` types.
         * @return Constructed matrix
         *
         *     Peanut::Matrix<int, 4, 1> a{1,2,3,4};
         *     Peanut::Matrix<int, 4, 1> b{5,6,7,8};
         *     Peanut::Matrix<int, 4, 1> c{9,10,11,12};
         *
         *     auto introwmat = Peanut::Matrix<int, 4, 3>::from_cols(a, b, c);
         *     // 1 5 9
         *     // 2 6 10
         *     // 3 7 11
         *     // 4 8 12
         *
         */
        template <typename ...CList>
            requires std::conjunction_v<std::is_same<Matrix<Type, Row, 1>, CList>...> &&
                     (sizeof...(CList) == Col)
        static Matrix from_cols(CList ... clist){
            Matrix<Type, Row, Col> ret;
            int c = 0;
            for(const Matrix<Type, Row, 1> p : {clist...}){
                for(int r=0;r<Row;r++){
                    ret.m_data[r*C+c] = p.m_data[r];
                }
                c++;
            }
            return ret;
        }

        /**
         * @brief Implementation of `MatrixExpr::operator()` which returns rvalue.
         * @param r Row index.
         * @param c Column index.
         * @return Rvalue of an element in \p r 'th Row and \p c 'th column.
         */
        INLINE T operator()(Index r, Index c) const{
            return m_data[r*C+c];
        }

        /**
         * @brief Get a reference of element in \p r 'th row and \p c 'th column.
         *        Note that `Peanut` allows to access lvalue for an evaluated
         *        matrix only.
         * @param r Row index.
         * @param c Column index.
         * @return Reference of an element in \p r 'th Row and \p c 'th column.
         */
        INLINE T& operator()(Index r, Index c) {
            return m_data[r*C+c];
        }

        /**
         * @brief Get a \p idx 'th row as a `Matrix<Type, 1, Col>` type.
         * @param idx Row index.
         * @return `Matrix<Type, 1, Col>` instance.
         */
        Matrix<Type, 1, Col> get_row(Index idx) const{
            Matrix<Type, 1, Col> ret;
            memcpy(ret.m_data.data(), &(m_data[idx*C]), sizeof(Type)*Col);
            return ret;
        }

        /**
         * @brief Set a \p idx 'th row as a given argument \p row .
         * @param idx Row index.
         * @param row Row matrix which will be assigned to the r'th row of the matrix.
         */
        void set_row(Index idx, const Matrix<Type, 1, Col> &row){
            memcpy(&(m_data[idx*C]), row.m_data.data(), sizeof(Type)*Col);
        }

        /**
         * @brief Get a \p idx 'th column as a `Matrix<Type, Row, 1>` type.
         * @param idx Column index.
         * @return `Matrix<Type, Row, 1>` instance.
         */
        Matrix<Type, Row, 1> get_col(Index idx) const{
            Matrix<Type, Row, 1> ret;
            for(int i=0;i<Row;i++){
                ret.m_data[i] = m_data[i*C+idx];
            }
            return ret;
        }

        /**
         * @brief Set a \p idx 'th column as a given argument \p col .
         * @param idx Column index.
         * @param row Column matrix which will be assigned to the idx'th column of the matrix.
         */
        void set_col(Index idx, const Matrix<Type, Row, 1> &col){
            for(int i=0;i<Row;i++){
                m_data[i*C+idx] = col.m_data[i];
            }
        }

        /**
         * @brief Evaluation expressions and return as a `Matrix` instance.
         *        Note that every matrix expression classes must implement this
         *        method even though it is not a method of `MatrixExpr`.
         * @param Evaluated matrix (reference output)
         */
        void eval(Matrix<Type, Row, Col> &_result) const{
            _result.m_data = m_data;
        }

        // =============== Features for vector usage begins ================

        /**
         * @brief Subscript operator available only for vector usage
         *        (i.e., Row==1 or Col==1)
         * @param i Index
         * @return \p T type i'th element data.
         */
        INLINE T operator[](Index i) const
            requires (Row==1) || (Col==1){
            return m_data[i];
        }

        /**
         * @brief Subscript operator available only for vector usage.
         *        (i.e., Row==1 or Col==1)
         * @param i Index
         * @return Reference of i'th element.
         */
        INLINE T& operator[](Index i)
            requires (Row==1) || (Col==1){
            return m_data[i];
        }

        /**
         * @brief Dot product available only for vector usage.
         *        (i.e., Row==1 or Col==1)
         * @param vec Equal-type matrix(vector).
         * @return T type dot product result.
         */
        T dot(const Matrix &vec) const requires (Row==1) || (Col==1){
            T ret = t_0;
            for(int i=0;i<Row*Col;i++){
                ret += (vec.m_data[i] * m_data[i]);
            }
            return ret;
        }

        /**
         * @brief L2 distance available only for vector usage.
         *        (i.e., Row==1 or Col==1)
         * @return Float l2 distance of the vector.
         */
        Float length() const requires (Row==1) || (Col==1){
            T ret = t_0;
            for(int i=0;i<Row*Col;i++){
                ret += (m_data[i] * m_data[i]);
            }
            return Peanut::sqrt(ret);
        }

        /**
         * @brief Vector normalization available only for vector usage.
         *        (i.e., Row==1 or Col==1)
         * @return Normalized Float matrix(vector).
         */
        Matrix<Float, Row, Col> normalize() const requires (Row==1) || (Col==1){
            Matrix<Float, Row, Col> ret;
            const Float len = length();
            for(int i=0;i<Row*Col;i++){
                 ret[i] = static_cast<Float>((*this)[i]) / len;
            }
            return ret;
        }

        /**
         * @brief Return max element, available only for vector usage.
         *        (i.e., Row==1 or Col==1)
         * @return Max element in the vector.
         */
        T max() const requires (Row==1) || (Col==1){
            return *std::max_element(m_data.begin(), m_data.end());
        }

        /**
         * @brief Return min element, available only for vector usage.
         *        (i.e., Row==1 or Col==1)
         * @return Min element in the vector.
         */
        T min() const requires (Row==1) || (Col==1){
            return *std::min_element(m_data.begin(), m_data.end());
        }

        /**
         * @brief Cross product available for 3-size vectors.
         * @return Cross-product result
         */
        static Matrix cross(const Matrix &m1, const Matrix &m2)
            requires (Row==1 && Col==3) || (Row==3 && Col==1){
            return {m1[1] * m2[2] - m1[2] * m2[1],
                    m1[2] * m2[0] - m1[0] * m2[2],
                    m1[0] * m2[1] - m1[1] * m2[0]};
        }

        /**
         * @brief L2 distance available only for vector usage.
         *        (i.e., Row==1 or Col==1)
         * @return Float l2 distance of given vectors.
         */
        static Float L2(const Matrix &m1, const Matrix &m2)
        requires (Row==1) || (Col==1){
            T ret = t_0;
            for(int i=0;i<Row*Col;i++){
                ret += std::pow(m1[i] - m2[i], 2);
            }
            return std::sqrt(ret);
        }

        // =============== Features for vector usage ends ================

        /**
         * @brief An implementation of `operator<<`
         * @return Given std::ostream
         */
        friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix) {
            for(int i=0;i< R * C;i++){
                os << matrix.m_data[i]<<" ";
            }
            return os;
        }

        /**
         * @brief Helper function for Gaussian elimination. It performs row
         *        subtraction after multiplying given scalar to the row
         *        (i.e., `r1 = r1 - (r2 * scalar)`).
         * @param r1 Row index.
         * @param r2 Row index.
         * @param scalar Scalar which will be multiplied to \p r2 'th Row.
         */
        void subtract_row(Index r1, Index r2, T scalar){
            for(int i=0;i< C;i++){
                (*this)(r1, i) -= scalar * (*this)(r2, i);
            }
        }

        /**
         * @brief Calculate a determinant by recursively calculate determinants
         *        of submatrices.
         * @return Determinant of the matrix.
         */
        constexpr T det() const requires is_square_v<Matrix>{
            if constexpr(C ==1){
                return m_data[0];
            }
            else if constexpr (C ==2){
                return m_data[0] * m_data[C+1] - m_data[1] * m_data[C];
            }
            else{
                T ret = static_cast<T>(0);

                for_<C>([&] (auto c) {
                    Matrix<T, R-1, C-1> submat;
                    SubMat<0, c.value>(*this).eval(submat);
                    ret += (c.value % 2 ? -1 : 1) * m_data[c.value] * submat.det();
                });
                return ret;
            }
        }

        // Matrix data
        std::array<T, R*C> m_data;
        // union {
        //     std::array<T, R * C> d1;
        //     T d2[R][C];
        // } m_data;

    private:
        static constexpr T t_1 = static_cast<T>(1);
        static constexpr T t_0 = static_cast<T>(0);
    };
}
