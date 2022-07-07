# Peanut

[![Unit Test](https://github.com/pjessesco/peanut/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/pjessesco/peanut/actions/workflows/cmake.yml) [![Deploy doxygen documents](https://github.com/pjessesco/peanut/actions/workflows/docs.yml/badge.svg?branch=main)](https://github.com/pjessesco/peanut/actions/workflows/docs.yml)

**Peanut** is a header-only matrix library using C++20 without any external dependencies, (except unit test), following a `expression templates` concept. It first constructs matrix expression as a user provides, and evaluates it using an automatically generated code in a compile-time.

```
Matrix<int, 4, 4> mat1{1,2,3,4,
                       1,2,3,4,
                       1,2,3,4,
                       1,2,3,4};

Matrix<int, 3, 5> mat2{1,2,3,4,5,
                       1,2,3,4,5,
                       1,2,3,4,5};

// Peanut first build expression tree,
// `result` type is `MatrixMult<MatrixSubtract<MatrixSum<Matrix<int, 4, 4>, MatrixInverse<Matrix<int, 4, 4>>>, MatrixMinor<Matrix<int, 4, 4>>>, MatrixSub<2, 1, MatrixMult<MatrixTranspose<Matrix<int, 3, 5>>, Matrix<int, 3, 5>>>>`.
auto result = (mat1 + Inverse(mat1) - Minor(mat1)) * SubMat<2, 1>(T(mat2) * mat2);

// then evaluate it when `eval()` is called or assigned to `Matrix` type variable.
/* Matrix<int, 4, 4> */ auto e1 = result.eval();
Matrix<int, 4, 4> e2 = result;
```

### Features
- Arbitrary size matrix expression
- Lazy evaluation
- Unit test

### Usage
Copy `Peanut` directory to your project, and add and its path to the include path. If you're using CMake, add :

    include_directories(Peanut)

Then include as below :

    #include "Matrix.h"

Refer [documentation](https://pjessesco.github.io/peanut/) for more information.
