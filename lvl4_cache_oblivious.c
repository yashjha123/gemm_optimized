// idealized from https://math.mit.edu/~stevenj/18.335/oblivious-matmul-handout.pdf

#include "include/naive_gemm.h"
#include "include/optimized_gemm.h"

#define index(i, j) ((i) * N + (j))

#define LEAF 32

// C[M×N] += A[M×K] * B[K×N], where C is row-major with leading dims lda/ldb/ldc.
static void mm_add(const int *A, int lda, const int *B, int ldb, int *C,
                      int ldc, int M, int N, int K) {
    if (M <= LEAF && N <= LEAF && K <= LEAF) { // base case (gets vectorized)
        for (int i = 0; i < M; ++i) {
            for (int k = 0; k < K; ++k) {
                int a = A[i * lda + k];
                for (int j = 0; j < N; ++j) {
                    C[i * ldc + j] += a * B[k * ldb + j];
                }
            }
        }
        return;
    }

    if (M >= N && M >= K) { // split M into two halves (M means rows)
        int M1 = M / 2, M2 = M - M1;
        mm_add(A, lda, B, ldb, C, ldc, M1, N, K);  // top
        mm_add(A + M1 * lda, lda, B, ldb, C + M1 * ldc, ldc, M2, N,K);  // bottom
    } else if (N >= M && N >= K) { // split N into two halves (N means columns)
        int N1 = N / 2, N2 = N - N1;
        mm_add(A, lda, B, ldb, C, ldc, M, N1, K);            // left
        mm_add(A, lda, B + N1, ldb, C + N1, ldc, M, N2, K);  // right
    } else {                                                    // else split K into two halves
        int K1 = K / 2, K2 = K - K1;
        mm_add(A, lda, B, ldb, C, ldc, M, N, K1);
        mm_add(A + K1, lda, B + K1 * ldb, ldb, C, ldc, M, N, K2);
    }
}

void cache_oblivious_matrix_multiply(int *A, int *B, int *C) {
    mm_add(A, N, B, N, C, N, N, N, N);
}
