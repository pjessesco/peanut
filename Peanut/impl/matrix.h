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
#include <array>
#include <cassert>
#include <initializer_list>
#include <iostream>
#include <type_traits>

// Peanut headers
#include "common.h"
#include "matrix_type_traits.h"

// Dependencies headers

namespace Peanut {

    /**
     * @brief Base class of every matrix expressions in Peanut.
     * @details Peanut implements matrix expression by following CRTP concept
     *          to achieve a static polymorphism. Every derived class should
     *          follow its interface even they are not forced by the compiler
     *          (i.e., pure virtual method). Also they MUST provide their data
     *          type via `using Type = XXX`, and implement `eval()` method for
     *          matrix evaluation. See `Matrix<T, Row, Col>::eval()` for more
     *          explanation.
     * @tparam E Derived class. `MatrixExpr`
     */
    template <typename E>
    struct MatrixExpr{

        /**
         * @brief Get element in \p r 'th row and \p c 'th column.
         * @details It is also used for evaluation of an arbitrary matrix expression
         *          by recursively calling in derived classes. See also
         *          `Matrix::Matrix(const MatrixExpr<E> &expr)` and
         *          `Matrix::elem(Index r, Index c)` and other derived classes.
         * @param r Row index.
         * @param c Col index.
         * @return Rvalue of an element in \p r 'th row and \p c 'th column.
         */
        inline auto elem(Index r, Index c) const{
            return static_cast<const E&>(*this).elem(r, c);
        }

        /**
         * @brief Row size of the expression \p E.
         */
        static constexpr Index row = E::row;

        /**
         * @brief Column size of the expression \p E.
         */
        static constexpr Index col = E::col;
    };

    /**
     * @brief Basic matrix class.
     * @tparam T Data type.
     * @tparam Row Row size.
     * @tparam Col Col size.
     */
    template<typename T, Index Row, Index Col> requires std::is_arithmetic_v<T> && (Row > 0) && (Col > 0)
    struct Matrix : public MatrixExpr<Matrix<T, Row, Col>>{

        /**
         * @brief Data type. See the detailed description of \p MatrixExpr.
         */
        using Type = T;

        /**
         * @brief Row size of the matrix.
         */
        static constexpr Index row = Row;

        /**
         * @brief Column size of the matrix.
         */
        static constexpr Index col = Col;

        /**
         * @brief Constructor without any parameters initialize to zero matrix.
         */
        Matrix() {m_data.d1.fill(t_0);}

        /**
         * @brief Constructor with initializer list.
         * @param tlist Initializer list with \p T types.
         */
        template <typename ...TList> requires std::conjunction_v<std::is_same<T, TList>...>
        Matrix(TList ... tlist) : m_data{std::forward<T>(tlist)...} {}

        /**
         * @brief Constructor with std::array.
         * @param data STL Array having `T` type and \p Row * \p Col size.
         */
        explicit Matrix(const std::array<T, Row * Col> &data) : m_data{data} {}

        /**
         * @brief Constructor with std::vector.
         * @param data STL vector having `T` type.
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
            for(Index r=0;r<Row;r++){
                for(Index c=0;c<Col;c++){
                    elem(r, c) = expr.elem(r, c);
                }
            }
        }

        /**
         * @brief Equivalent with `Matrix::Matrix()`, but for explicit purpose.
         * @return Zero matrix with given \p Row and \p Col .
         */
        static Matrix zeros() {return Matrix();}

        /**
         * @brief Construct identity matrix. Available only for square matrix case.
         * @return Identity matrix with given \p Row and \p Col .
         */
        static Matrix identity() requires is_square_v<Matrix> {
            Matrix a;
            a.m_data.d1.fill(t_0);
            for (Index i = 0; i < Row; i++) {
                a.m_data.d2[i][i] = t_1;
            }
            return a;
        }

        /**
         * @brief Implementation of `MatrixExpr::elem()` which returns rvalue.
         * @param r Row index.
         * @param c Col index.
         * @return Rvalue of an element in \p r 'th row and \p c 'th column.
         */
        inline T elem(Index r, Index c) const{
            assert((0<=r) && (r < Row) && (0<=c) && (c < Col));
            return m_data.d2[r][c];
        }

        /**
         * @brief Get a reference of element in \p r 'th row and \p c 'th column.
         *        Note that `Peanut` allows to access lvalue for an evaluated
         *        matrix only.
         * @param r Row index.
         * @param c Col index.
         * @return Reference of an element in \p r 'th row and \p c 'th column.
         */
        inline T& elem(Index r, Index c) {
            assert((0<=r) && (r < Row) && (0<=c) && (c < Col));
            return m_data.d2[r][c];
        }

        /**
         * @brief Evaluation expressions and return as a `Matrix` instance.
         *        Note that every matrix expression classes must implement this
         *        method even though it is not a method of `MatrixExpr`.
         * @return Evaluated matrix
         */
        inline Matrix<Type, row, col> eval() const{
            return Matrix<Type, row, col>(*this);
        }

        /**
         * @brief An implementation of `operator<<`
         * @return Given std::ostream
         */
        friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix) {
            for(int i=0;i<Row*Col;i++){
                os << matrix.m_data.d1[i]<<" ";
            }
            return os;
        }

//        Matrix<T, 1, col> row(Index r) const{
//
//        }

        /**
         * @brief Helper function for Gaussian elimination. It performs row
         *        subtraction after multiplying given scalar to the row
         *        (i.e., `r1 = r1 - (r2 * scalar)`).
         * @param r1 Row index.
         * @param r2 Row index.
         * @param scalar Scalar which will be multiplied to \p r2 'th row.
         */
        void subtract_row(Index r1, Index r2, T scalar){
            for(int i=0;i<Col;i++){
                elem(r1, i) -= scalar * elem(r2, i);
            }
        }

        /**
         * @brief Perform brute-force Gaussian elimination, which elimiate
         *        left-most element per row by repeating `subtract_row()`.
         * @details Since it does not perform elimination efficiently, numerical
         *          issue may exists with a extremely large/small numbers.
         * @return Gaussian elimination-performed matrix.
         */
        Matrix<Float, Row, Col> gaussian_elem() const{
            Matrix<Float, Row, Col> ret = Cast<Float>(*this);
            for(int j=0;j<Row-1;j++){
                const Float denom = static_cast<Float>(ret.elem(j,j));
                if(is_zero(denom)){
                    continue;
                }
                for(int i=j+1;i<Row;i++){
                    const Float ratio = static_cast<Float>(ret.elem(i, j)) / denom;
                    ret.subtract_row(i, j, ratio);
                }
            }
            return ret;
        }

        /**
         * @brief Calculate a determinant by recursively calculate determinants
         *        of submatrices.
         * @return Determinant of the matrix.
         */
        constexpr T det() const requires is_square_v<Matrix>{
            if constexpr(Col==1){
                return elem(0, 0);
            }
            else if constexpr (Col==2){
                return elem(0, 0) * elem(1, 1) - elem(0, 1) * elem(1, 0);
            }
            else{
                T ret = static_cast<T>(0);
                for_<Col>([&] (auto c) {
                    ret += (c.value % 2 ? -1 : 1) * elem(0, c.value) * SubMat<0, c.value>(*this).eval().det();
                });
                return ret;
            }
        }

        /**
         * @brief Calculate a determinant by performing `gaussian_elem()` and
         *        multiplying diagonal terms. It is equivalent to `det()`
         *        theoretically, but numerical issue may exists.
         * @return Determinant of the matrix.
         */
        constexpr T det2() const requires is_square_v<Matrix>{
            if constexpr(Col==1){
                return elem(0, 0);
            }
            else if constexpr (Col==2){
                return elem(0, 0) * elem(1, 1) - elem(0, 1) * elem(1, 0);
            }
            auto upper_triangular = gaussian_elem();
            Float det = upper_triangular.elem(0, 0);
            for(int i=1;i<Row;i++){
                det *= upper_triangular.elem(i, i);
            }
            return det;
        }

    private:
        static constexpr T t_1 = static_cast<T>(1);
        static constexpr T t_0 = static_cast<T>(0);
        union {
            std::array<T, Row*Col> d1;
            T d2[Row][Col];
        } m_data;

    };
}
