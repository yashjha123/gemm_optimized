#include "include/naive.h"

#define index(i, j) ((i) * N + (j))

void matrix_multiply_i_j_k(int *A, int *B, int *C) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[index(i, j)] += A[index(i, k)] * B[index(k, j)];
            }
        }
    }
}

void matrix_multiply_k_i_j(int *A, int *B, int *C) {
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                C[index(i, j)] += A[index(i, k)] * B[index(k, j)];
            }
        }
    }
}

void matrix_multiply_j_k_i(int *A, int *B, int *C) {
    for (int j = 0; j < N; j++) {
        for (int k = 0; k < N; k++) {
            for (int i = 0; i < N; i++) {
                C[index(i, j)] += A[index(i, k)] * B[index(k, j)];
            }
        }
    }
}

void matrix_multiply_i_k_j(int *A, int *B, int *C) {
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            for (int j = 0; j < N; j++) {
                C[index(i, j)] += A[index(i, k)] * B[index(k, j)];
            }
        }
    }
}

void matrix_multiply_k_j_i(int *A, int *B, int *C) {
    for (int k = 0; k < N; k++) {
        for (int j = 0; j < N; j++) {
            for (int i = 0; i < N; i++) {
                C[index(i, j)] += A[index(i, k)] * B[index(k, j)];
            }
        }
    }
}

void matrix_multiply_j_i_k(int *A, int *B, int *C) {
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            for (int k = 0; k < N; k++) {
                C[index(i, j)] += A[index(i, k)] * B[index(k, j)];
            }
        }
    }
}
