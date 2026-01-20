#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
#include <functional>

#define EIGEN_DONT_VECTORIZE
#define ARMA_DONT_USE_BLAS
#define ARMA_DONT_USE_LAPACK

#include <Peanut/Peanut.h>
#include "eigen/Eigen/Core"
#include "armadillo-code/include/armadillo"
#include "bruteforce-matrix/matrix.h"

// ============================================================================
// Benchmark utilities
// ============================================================================

template<typename Func>
double measure_time_us(Func&& func, int iterations) {
    // Warmup
    for (int i = 0; i < 3; ++i) {
        func();
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
           / static_cast<double>(iterations);
}

void print_header(const std::string& benchmark_name) {
    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << " " << benchmark_name << "\n";
    std::cout << "============================================================\n";
    std::cout << std::setw(15) << "Library"
              << std::setw(20) << "Time (us)"
              << std::setw(15) << "Relative" << "\n";
    std::cout << "------------------------------------------------------------\n";
}

void print_result(const std::string& name, double time_us, double baseline_us) {
    std::cout << std::setw(15) << name
              << std::setw(20) << std::fixed << std::setprecision(3) << time_us
              << std::setw(15) << std::fixed << std::setprecision(2)
              << (time_us / baseline_us) << "x\n";
}

// ============================================================================
// Benchmark: Matrix Addition (Chained)
// ============================================================================

template<int N>
void benchmark_chained_addition(int iterations) {
    print_header("Chained Addition (" + std::to_string(N) + "x" + std::to_string(N) + ", 8 matrices)");

    double peanut_time, eigen_time, arma_time, naive_time;

    // Peanut
    {
        Peanut::Matrix<float, N, N> a;
        for (int i = 0; i < N*N; ++i) a.m_data[i] = static_cast<float>(i % 100);
        Peanut::Matrix<float, N, N> result;

        peanut_time = measure_time_us([&]() {
            result = a + a + a + a + a + a + a + a;
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Eigen
    {
        Eigen::Matrix<float, N, N> a;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                a(i, j) = static_cast<float>((i*N + j) % 100);
        Eigen::Matrix<float, N, N> result;

        eigen_time = measure_time_us([&]() {
            result = a + a + a + a + a + a + a + a;
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Armadillo
    {
        arma::fmat a(N, N);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                a(i, j) = static_cast<float>((i*N + j) % 100);
        arma::fmat result(N, N);

        arma_time = measure_time_us([&]() {
            result = a + a + a + a + a + a + a + a;
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Naive (no expression templates)
    {
        MatrixBT<float, N, N> a;
        for (int i = 0; i < N*N; ++i) a.m_data[i] = static_cast<float>(i % 100);
        MatrixBT<float, N, N> result;

        naive_time = measure_time_us([&]() {
            result = a + a + a + a + a + a + a + a;
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    print_result("Peanut", peanut_time, peanut_time);
    print_result("Eigen", eigen_time, peanut_time);
    print_result("Armadillo", arma_time, peanut_time);
    print_result("Naive", naive_time, peanut_time);
}

// ============================================================================
// Benchmark: Matrix Multiplication
// ============================================================================

template<int N>
void benchmark_multiplication(int iterations) {
    print_header("Matrix Multiplication (" + std::to_string(N) + "x" + std::to_string(N) + ")");

    double peanut_time, eigen_time, arma_time, naive_time;

    // Peanut
    {
        Peanut::Matrix<float, N, N> a, b;
        for (int i = 0; i < N*N; ++i) {
            a.m_data[i] = static_cast<float>(i % 100);
            b.m_data[i] = static_cast<float>((i + 1) % 100);
        }
        Peanut::Matrix<float, N, N> result;

        peanut_time = measure_time_us([&]() {
            result = a * b;
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Eigen
    {
        Eigen::Matrix<float, N, N> a, b;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                a(i, j) = static_cast<float>((i*N + j) % 100);
                b(i, j) = static_cast<float>((i*N + j + 1) % 100);
            }
        }
        Eigen::Matrix<float, N, N> result;

        eigen_time = measure_time_us([&]() {
            result = a * b;
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Armadillo
    {
        arma::fmat a(N, N), b(N, N);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                a(i, j) = static_cast<float>((i*N + j) % 100);
                b(i, j) = static_cast<float>((i*N + j + 1) % 100);
            }
        }
        arma::fmat result(N, N);

        arma_time = measure_time_us([&]() {
            result = a * b;
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Naive
    {
        MatrixBT<float, N, N> a, b;
        for (int i = 0; i < N*N; ++i) {
            a.m_data[i] = static_cast<float>(i % 100);
            b.m_data[i] = static_cast<float>((i + 1) % 100);
        }
        MatrixBT<float, N, N> result;

        naive_time = measure_time_us([&]() {
            result = a * b;
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    print_result("Peanut", peanut_time, peanut_time);
    print_result("Eigen", eigen_time, peanut_time);
    print_result("Armadillo", arma_time, peanut_time);
    print_result("Naive", naive_time, peanut_time);
}

// ============================================================================
// Benchmark: Transpose
// ============================================================================

template<int N>
void benchmark_transpose(int iterations) {
    print_header("Transpose (" + std::to_string(N) + "x" + std::to_string(N) + ")");

    double peanut_time, eigen_time, arma_time, naive_time;

    // Peanut
    {
        Peanut::Matrix<float, N, N> a;
        for (int i = 0; i < N*N; ++i) a.m_data[i] = static_cast<float>(i % 100);
        Peanut::Matrix<float, N, N> result;

        peanut_time = measure_time_us([&]() {
            result = Peanut::T(a);
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Eigen
    {
        Eigen::Matrix<float, N, N> a;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                a(i, j) = static_cast<float>((i*N + j) % 100);
        Eigen::Matrix<float, N, N> result;

        eigen_time = measure_time_us([&]() {
            result = a.transpose();
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Armadillo
    {
        arma::fmat a(N, N);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                a(i, j) = static_cast<float>((i*N + j) % 100);
        arma::fmat result(N, N);

        arma_time = measure_time_us([&]() {
            result = a.t();
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Naive
    {
        MatrixBT<float, N, N> a;
        for (int i = 0; i < N*N; ++i) a.m_data[i] = static_cast<float>(i % 100);
        MatrixBT<float, N, N> result;

        naive_time = measure_time_us([&]() {
            result = a.transpose();
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    print_result("Peanut", peanut_time, peanut_time);
    print_result("Eigen", eigen_time, peanut_time);
    print_result("Armadillo", arma_time, peanut_time);
    print_result("Naive", naive_time, peanut_time);
}

// ============================================================================
// Benchmark: Complex Expression  a + a - (a * a) * (a + a - a * a)
// ============================================================================

template<int N>
void benchmark_complex_expression(int iterations) {
    print_header("Complex Expression (" + std::to_string(N) + "x" + std::to_string(N) + "): a+a-(a*a)*(a+a-a*a)");

    double peanut_time, eigen_time, arma_time, naive_time;

    // Peanut
    {
        Peanut::Matrix<float, N, N> a;
        for (int i = 0; i < N*N; ++i) a.m_data[i] = static_cast<float>((i % 100) * 0.01f);
        Peanut::Matrix<float, N, N> result;

        peanut_time = measure_time_us([&]() {
            result = a + a - (a * a) * (a + a - a * a);
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Eigen
    {
        Eigen::Matrix<float, N, N> a;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                a(i, j) = static_cast<float>(((i*N + j) % 100) * 0.01f);
        Eigen::Matrix<float, N, N> result;

        eigen_time = measure_time_us([&]() {
            result = a + a - (a * a) * (a + a - a * a);
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Armadillo
    {
        arma::fmat a(N, N);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                a(i, j) = static_cast<float>(((i*N + j) % 100) * 0.01f);
        arma::fmat result(N, N);

        arma_time = measure_time_us([&]() {
            result = a + a - (a * a) * (a + a - a * a);
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Naive
    {
        MatrixBT<float, N, N> a;
        for (int i = 0; i < N*N; ++i) a.m_data[i] = static_cast<float>((i % 100) * 0.01f);
        MatrixBT<float, N, N> result;

        naive_time = measure_time_us([&]() {
            result = a + a - (a * a) * (a + a - a * a);
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    print_result("Peanut", peanut_time, peanut_time);
    print_result("Eigen", eigen_time, peanut_time);
    print_result("Armadillo", arma_time, peanut_time);
    print_result("Naive", naive_time, peanut_time);
}

// ============================================================================
// Benchmark: A^T * A
// ============================================================================

template<int M, int N>
void benchmark_ata(int iterations) {
    print_header("A^T * A (" + std::to_string(M) + "x" + std::to_string(N) + " -> "
                 + std::to_string(N) + "x" + std::to_string(N) + ")");

    double peanut_time, eigen_time, arma_time, naive_time;

    // Peanut
    {
        Peanut::Matrix<float, M, N> a;
        for (int i = 0; i < M*N; ++i) a.m_data[i] = static_cast<float>(i % 100);
        Peanut::Matrix<float, N, N> result;

        peanut_time = measure_time_us([&]() {
            result = Peanut::T(a) * a;
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Eigen
    {
        Eigen::Matrix<float, M, N> a;
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j)
                a(i, j) = static_cast<float>((i*N + j) % 100);
        Eigen::Matrix<float, N, N> result;

        eigen_time = measure_time_us([&]() {
            result = a.transpose() * a;
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Armadillo
    {
        arma::fmat a(M, N);
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j)
                a(i, j) = static_cast<float>((i*N + j) % 100);
        arma::fmat result(N, N);

        arma_time = measure_time_us([&]() {
            result = a.t() * a;
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    // Naive
    {
        MatrixBT<float, M, N> a;
        for (int i = 0; i < M*N; ++i) a.m_data[i] = static_cast<float>(i % 100);
        MatrixBT<float, N, N> result;

        naive_time = measure_time_us([&]() {
            result = a.transpose() * a;
        }, iterations);

        volatile float check = result(0, 0);
        (void)check;
    }

    print_result("Peanut", peanut_time, peanut_time);
    print_result("Eigen", eigen_time, peanut_time);
    print_result("Armadillo", arma_time, peanut_time);
    print_result("Naive", naive_time, peanut_time);
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "================================================================\n";
    std::cout << "              Peanut Matrix Library Benchmark\n";
    std::cout << "================================================================\n";
    std::cout << "Comparing: Peanut vs Eigen vs Armadillo vs Naive\n";
    std::cout << "(Eigen vectorization disabled for fair comparison)\n";

    // Small matrices (4x4) - high iteration count
    benchmark_chained_addition<4>(100000);
    benchmark_multiplication<4>(100000);
    benchmark_transpose<4>(100000);
    benchmark_complex_expression<4>(100000);

    // Medium matrices (32x32)
    benchmark_chained_addition<32>(10000);
    benchmark_multiplication<32>(10000);
    benchmark_transpose<32>(10000);
    benchmark_complex_expression<32>(1000);

    // Larger matrices (64x64)
    benchmark_chained_addition<64>(5000);
    benchmark_multiplication<64>(1000);
    benchmark_transpose<64>(5000);
    benchmark_complex_expression<64>(500);

    // A^T * A benchmark
    benchmark_ata<64, 32>(5000);
    benchmark_ata<128, 64>(1000);

    std::cout << "\n================================================================\n";
    std::cout << "                    Benchmark Complete\n";
    std::cout << "================================================================\n";

    return 0;
}
