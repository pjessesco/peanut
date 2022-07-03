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
#include <type_traits>

// Peanut headers

// Dependencies headers

namespace Peanut {

    template <typename E> struct MatrixExpr;

    /**
     * @brief Compile-time checking structure if given type is Peanut's matrix expression type.
     * @details constexpr `value` is true if given type inherits \pMatrixExpr, false otherwise.
     *          Note that every matrix expression type inherits \pMatrixExpr in Peanut.
     * @tparam E Arbitrary type.
     */
    template <typename E>
        struct is_matrix{
        static constexpr bool value = std::is_base_of_v<MatrixExpr<E>, E>;
    };

    /**
     * @brief Helper variable template for \is_matrix<E>.
     */
    template <typename E>
    constexpr bool is_matrix_v = is_matrix<E>::value;

    // =========================================================================

    /**
     * @brief Compile-time checking structure if two Peanut matrix expression type has same size.
     * @details constexpr `value` is true if rows and cols of \pE1 and \pE2 are equal.
     * @tparam E1 Arbitrary Peanut matrix expression.
     * @tparam E2 Arbitrary Peanut matrix expression.
     */
    template <typename E1, typename E2> requires is_matrix_v<E1> && is_matrix_v<E2>
    struct is_equal_size_mat{
        static constexpr bool value = (E1::row == E2::row && E1::col == E2::col);
    };

    /**
     * @brief Helper variable template for \is_equal_size_mat<E1, E2>.
     */
    template <typename E1, typename E2>
    constexpr bool is_equal_size_mat_v = is_equal_size_mat<E1, E2>::value;

    // =========================================================================

    /**
     * @brief Compile-time checking structure if given Peanut matrix expression type is square.
     * @details constexpr `value` is true if \pE's row equals to its col.
     * @tparam E Arbitrary Peanut matrix expression.
     */
    template <typename E> requires is_matrix_v<E>
    struct is_square{
        static constexpr bool value = E::row == E::col;
    };

    /**
     * @brief Helper variable template for \pis_square_v<E>.
     */
    template <typename E>
    constexpr bool is_square_v = is_square<E>::value;

    // =========================================================================

    /**
     * @brief Compile-time checking structure if given Peanut matrix expression's data type is same.
     * @details constexpr `value` is true if \E1's data type and \pE2's data type are same.
     * @tparam E1 Arbitrary Peanut matrix expression.
     * @tparam E2 Arbitrary Peanut matrix expression.
     */
    template <typename E1, typename E2> requires is_matrix_v<E1> && is_matrix_v<E2>
    struct is_equal_type{
        static constexpr bool value = std::is_same_v<typename E1::Type, typename E2::Type>;
    };

    /**
     * @brief Helper variable template for \pis_equal_type<E1, E2>.
     */
    template <typename E1, typename E2>
    constexpr bool is_equal_type_v = is_equal_type<E1, E2>::value;

    // =========================================================================

    /**
     * @brief Compile-time checking structure if given Peanut matrix expressions
     *        have same size and type.
     * @details Equivalent to `is_equal_type_v<E1, E2> && is_equal_size_mat_v<E1, E2>`.
     * @tparam E1 Arbitrary Peanut matrix expression.
     * @tparam E2 Arbitrary Peanut matrix expression.
     */
    template <typename E1, typename E2> requires is_matrix_v<E1> && is_matrix_v<E2>
    struct is_equal_type_size{
        static constexpr bool value = (is_equal_type_v<E1, E2> && is_equal_size_mat_v<E1, E2>);
    };

    /**
     * @brief Helper variable template for \pis_equal_type_size<E1, E2>.
     */
    template <typename E1, typename E2>
    constexpr bool is_equal_type_size_v = is_equal_type_size<E1, E2>::value;
}