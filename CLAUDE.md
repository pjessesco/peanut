# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
# Configure with tests enabled
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DTEST=ON

# Build
cmake --build build --config Debug

# Run tests
./build/test/PeanutTest

# Run a single test (uses Catch2)
./build/test/PeanutTest "test name here"

# List all tests
./build/test/PeanutTest --list-tests
```

## Architecture

Peanut is a header-only C++20 matrix library implementing **expression templates** for lazy evaluation. All code is in `include/Peanut/`.

### Core Concepts

**Expression Templates**: Matrix operations don't immediately compute results. Instead, they construct a type that encodes the operation. Evaluation only happens when:
- `eval()` is called on the expression
- The expression is assigned to a `Matrix<T, R, C>` variable

For example, `(mat1 + mat2) * mat3` produces type `MatrixMult<MatrixSum<Matrix<...>, Matrix<...>>, Matrix<...>>` and evaluation traverses this type tree.

### Key Components

- `MatrixExpr<E>` (`impl/matrix.h`): CRTP base class for all expressions. Derived classes must implement `operator()(Index r, Index c)` for element access and `eval(Matrix&)` for evaluation.

- `Matrix<T, R, C>` (`impl/matrix.h`): Concrete matrix storage with compile-time dimensions. Arithmetic type `T`, row count `R`, and column count `C` are template parameters.

- **Binary expressions** (`impl/binary_expr/`): `MatrixSum`, `MatrixSubtract`, `MatrixMult`, `MatrixMultScalar`, `MatrixDivScalar`, `MatrixEMult`, `MatrixEDiv`

- **Unary expressions** (`impl/unary_expr/`): `MatrixTranspose` (`T()`), `MatrixInverse` (`Inverse()`), `MatrixNegation`, `Minor()`, `Cofactor()`, `Adjugate()`, `Block<>`, `SubMat<>`, `Cast<>`

### Performance Note

`MatrixMult` eagerly evaluates its operands during construction to avoid repeated traversal of sub-expressions. Other operations remain lazy.

### Type Traits

`impl/matrix_type_traits.h` provides compile-time checks: `is_matrix_v`, `is_square_v`, `is_equal_type_size_v`, etc.

### Compile-Time Loop

`for_<N>(func)` in `impl/common.h` enables compile-time iteration with a callable receiving `num<I>` for loop index access.
