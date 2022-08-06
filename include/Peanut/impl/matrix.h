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
         *          `Matrix::elem(Index r, Index c)` and other derived classes.
         * @param r R index.
         * @param c C index.
         * @return Rvalue of an element in \p r 'th Row and \p c 'th column.
         */
        inline auto elem(Index r, Index c) const{
            return static_cast<const E&>(*this).elem(r, c);
        }

        /**
         * @brief R size of the expression \p E.
         */
        static constexpr Index Row = E::Row;

        /**
         * @brief Column size of the expression \p E.
         */
        static constexpr Index Col = E::Col;
    };

    /**
     * @brief Basic matrix class.
     * @tparam T Data type.
     * @tparam R Row size.
     * @tparam C Column size.
     */
    template<typename T, Index R, Index C> requires std::is_arithmetic_v<T> && (R > 0) && (C > 0)
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
         * @brief Constructor without any parameters initialize to zero matrix.
         */
        Matrix() {m_data.d1.fill(t_0);}

        /**
         * @brief Constructor with initializer list.
         * @param tlist Initializer list with \p T types.
         */
        template <typename ...TList>
            requires std::conjunction_v<std::is_same<T, TList>...> &&
                     (sizeof...(TList) == Row*Col)
        Matrix(TList ... tlist) : m_data{std::forward<T>(tlist)...} {}

        /**
         * @brief Constructor with std::array.
         * @param data A std::array having `T` type and \p R * \p C size.
         */
        explicit Matrix(const std::array<T, R * C> &data) : m_data{data} {}

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
                    m_data.d2[r][c] = expr.elem(r, c);
                }
            }
        }

        /**
         * @brief Equivalent with `Matrix::Matrix()`, but for explicit purpose.
         * @return Zero matrix with given \p R and \p C .
         */
        static Matrix zeros() {return Matrix();}

        /**
         * @brief Construct identity matrix. Available only for square matrix case.
         * @return Identity matrix with given \p R and \p C .
         */
        static Matrix identity() requires is_square_v<Matrix> {
            Matrix a;
            a.m_data.d1.fill(t_0);
            for (Index i = 0; i < R; i++) {
                a.m_data.d2[i][i] = t_1;
            }
            return a;
        }

        /**
         * @brief Implementation of `MatrixExpr::elem()` which returns rvalue.
         * @param r Row index.
         * @param c Column index.
         * @return Rvalue of an element in \p r 'th Row and \p c 'th column.
         */
        inline T elem(Index r, Index c) const{
            assert((0<=r) && (r < R) && (0<=c) && (c < C));
            return m_data.d2[r][c];
        }

        /**
         * @brief Get a reference of element in \p r 'th row and \p c 'th column.
         *        Note that `Peanut` allows to access lvalue for an evaluated
         *        matrix only.
         * @param r Row index.
         * @param c Column index.
         * @return Reference of an element in \p r 'th Row and \p c 'th column.
         */
        inline T& elem(Index r, Index c) {
            assert((0<=r) && (r < R) && (0<=c) && (c < C));
            return m_data.d2[r][c];
        }

        /**
         * @brief Get a \p idx 'th row as a `Matrix<Type, 1, Col>` type.
         * @param idx Row index.
         * @return `Matrix<Type, 1, Col>` instance.
         */
        Matrix<Type, 1, Col> get_row(Index idx) const{
            assert((0<=idx) && (idx < Row));
            Matrix<Type, 1, Col> ret;
            memcpy(ret.m_data.d2, m_data.d2[idx], sizeof(Type)*Col);
            return ret;
        }

        /**
         * @brief Set a \p idx 'th row as a given argument \p row .
         * @param idx Row index.
         * @param row Row matrix which will be assigned to the r'th row of the matrix.
         */
        void set_row(Index idx, const Matrix<Type, 1, Col> &row){
            assert((0<=idx) && (idx < R));
            memcpy(m_data.d2[idx], row.m_data.d2, sizeof(Type)*Col);
        }

        /**
         * @brief Get a \p idx 'th column as a `Matrix<Type, Row, 1>` type.
         * @param idx Column index.
         * @return `Matrix<Type, Row, 1>` instance.
         */
        Matrix<Type, Row, 1> get_col(Index idx) const{
            assert((0<=idx) && (idx < Col));
            Matrix<Type, Row, 1> ret;
            for(int i=0;i<Row;i++){
                ret.m_data.d1[i] = m_data.d2[i][idx];
            }
            return ret;
        }

        /**
         * @brief Set a \p idx 'th column as a given argument \p col .
         * @param idx Column index.
         * @param row Column matrix which will be assigned to the idx'th column of the matrix.
         */
        void set_col(Index idx, const Matrix<Type, Row, 1> &col){
            assert((0<=idx) && (idx < Col));
            for(int i=0;i<Row;i++){
                m_data.d2[i][idx] = col.m_data.d1[i];
            }
        }

        /**
         * @brief Evaluation expressions and return as a `Matrix` instance.
         *        Note that every matrix expression classes must implement this
         *        method even though it is not a method of `MatrixExpr`.
         * @return Evaluated matrix
         */
        inline Matrix<Type, Row, Col> eval() const{
            return Matrix<Type, Row, Col>(*this);
        }

        /**
         * @brief An implementation of `operator<<`
         * @return Given std::ostream
         */
        friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix) {
            for(int i=0;i< R * C;i++){
                os << matrix.m_data.d1[i]<<" ";
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
        Matrix<Float, R, C> gaussian_elem() const{
            Matrix<Float, R, C> ret = Cast<Float>(*this);
            for(int j=0;j< R -1;j++){
                const Float denom = static_cast<Float>(ret.elem(j,j));
                if(is_zero(denom)){
                    continue;
                }
                for(int i=j+1;i< R;i++){
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
            if constexpr(C ==1){
                return m_data.d2[0][0];
            }
            else if constexpr (C ==2){
                return m_data.d2[0][0] * m_data.d2[1][1] - m_data.d2[0][1] * m_data.d2[1][0];
            }
            else{
                T ret = static_cast<T>(0);
                for_<C>([&] (auto c) {
                    ret += (c.value % 2 ? -1 : 1) * m_data.d2[0][c.value] * SubMat<0, c.value>(*this).eval().det();
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
            if constexpr(C ==1){
                return m_data.d2[0][0];
            }
            else if constexpr (C ==2){
                return elem(0, 0) * elem(1, 1) - elem(0, 1) * elem(1, 0);
            }
            auto upper_triangular = gaussian_elem();
            Float det = upper_triangular.elem(0, 0);
            for(int i=1;i< R;i++){
                det *= upper_triangular.elem(i, i);
            }
            return det;
        }

        // Matrix data
        union {
            std::array<T, R * C> d1;
            T d2[R][C];
        } m_data;

    private:
        static constexpr T t_1 = static_cast<T>(1);
        static constexpr T t_0 = static_cast<T>(0);
    };
}
