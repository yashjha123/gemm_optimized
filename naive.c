#include "include/naive.h"

#define N 4
#define index(i, j) ((i) * N + (j))

void naive_matrix_multiply(int *A, int *B, int *C){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            for (int k = 0; k < N; k++){
                C[index(i, j)] += A[index(i, k)] * B[index(k, j)];
            }
        }
    }
}
