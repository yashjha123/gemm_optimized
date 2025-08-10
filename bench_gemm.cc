#include <benchmark/benchmark.h>

#include <vector>
#include <cstring>
#include <memory>
#include <cstdlib>
#include <iostream>

#include "include/utils.h"

extern "C" {
void optimized_matrix_multiply(int * A, int *B, int *C);
void naive_matrix_multiply(int *A, int *B, int *C);
extern int N;
// extern int MC;
// extern int NC;
// extern int block_size;
}

struct Matrix {
    int *p = nullptr;
    size_t n = 0;

    explicit Matrix(size_t size) : n(size) { reset(size); }

    ~Matrix() {
        if (p) {
            free(p);
        }
    }

    void reset(size_t size) {
        if (p) {
            free(p);
        }
        n = size;
        // std::cout << "Intialzing an array of " << n << " & " << n << std::endl;
        if (ALLOC_ALIGNED(p, n * n * sizeof(int))) {
            quick_exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < n * n; ++i) {
            p[i] = i;
        }
    }
};

static void
ReportGflops(benchmark::State &state) {
    const int N = static_cast<int>(state.range(0));
    const double flops = 2.0 * double(N) * double(N) * double(N);  // 2*N^3
    state.counters["GFLOPS"] = benchmark::Counter(
        flops, benchmark::Counter::kIsIterationInvariantRate);
}


static void BM_NaiveMM(benchmark::State &state) {
    const int rnN = static_cast<int>(state.range(0));
    N = rnN;

    Matrix A(N), B(N), C(N);
    std::memset(C.p, 0, N * N * sizeof(int));

    // Optional warmup
    naive_matrix_multiply(A.p, B.p, C.p);
    std::memset(C.p, 0, N * N * sizeof(int));
    for (auto _ : state) {
        state.PauseTiming();
        std::memset(C.p, 0, N * N * sizeof(int));
        state.ResumeTiming();

        naive_matrix_multiply(A.p, B.p, C.p);

        benchmark::DoNotOptimize(C.p);
        benchmark::ClobberMemory();
    }
    ReportGflops(state);
    // free(A.p);
    // free(B.p);
    // free(C.p);
}

static void BM_OptimizedMM(benchmark::State &state) {
    const int rnN = static_cast<int>(state.range(0));
    N = rnN;

    Matrix A(N), B(N), C(N);
    std::memset(C.p, 0, N * N * sizeof(int));

    optimized_matrix_multiply(A.p, B.p, C.p);
    std::memset(C.p, 0, N * N * sizeof(int));
    // int i = 0;
    for (auto _ : state) {
        // std::cout << i << std::endl;
        state.PauseTiming();
        std::memset(C.p, 0, N * N * sizeof(int));
        state.ResumeTiming();
        // std::cout << "Running" << std::endl;

        optimized_matrix_multiply(A.p, B.p, C.p);

        benchmark::DoNotOptimize(C.p);
        benchmark::ClobberMemory();
    }
    ReportGflops(state);
    // free(A.p);
    // free(B.p);
    // free(C.p);
}
BENCHMARK(BM_NaiveMM)
    ->ArgsProduct({{256,512,1024,2048}}) // tests for each of the sizes
    ->UseRealTime();


BENCHMARK(BM_OptimizedMM)
    ->ArgsProduct({{256,512,1024,2048}}) // tests for each of the sizes
    ->UseRealTime();


BENCHMARK_MAIN();
