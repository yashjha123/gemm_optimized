#include "constants.h"
#define index(i, j) ((i) * N + (j))

void naive_matrix_multiply(int *A, int *B, int *C);

void matrix_multiply_i_j_k(int *A, int *B, int *C);
void matrix_multiply_k_i_j(int *A, int *B, int *C);
void matrix_multiply_j_k_i(int *A, int *B, int *C);

void matrix_multiply_i_k_j(int *A, int *B, int *C);
void matrix_multiply_k_j_i(int *A, int *B, int *C);
void matrix_multiply_j_i_k(int *A, int *B, int *C);