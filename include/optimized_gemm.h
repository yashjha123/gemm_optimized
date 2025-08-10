#pragma once

#include <stdlib.h>

#include "constants.h"
#define index(i, j) ((i) * N + (j))

#ifdef __cplusplus // for google benchmark
extern "C" {
#endif

void optimized_matrix_multiply(int *A, int *B, int *C);
void naive_matrix_multiply(int *A, int *B, int *C);
void blocked_matrix_multiply(int *A, int *B, int *C);
void cache_oblivious_matrix_multiply(int *A, int *B, int *C);
extern int N;
// extern int MC;
// extern int NC;
// extern int block_size;

#ifdef __cplusplus
}
#endif