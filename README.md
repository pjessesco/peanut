# Peanut
**Peanut** is a header-only matrix library using C++20 without any external dependencies, (except unit test), following a `expression templates` concept. It first constructs matrix expression as a user provides, and evaluates it using an automatically generated code in a compile-time.

### Features
- Arbitrary size matrix expression
- Lazy evaluation
- Unit test (ongoing)

### Usage
Copy `Peanut` directory to your project, and add and its path to the include path. If you're using CMake, add :

    include_directories(Peanut)

Then include as below :

    #include "Matrix.h"

