#pragma once
#include "constants.h"
#define index(i, j) ((i) * N + (j))

void micro_level_matrix_multiply(int *A, int *B, int *C);
void blocked_matrix_multiply(int *A, int *B, int *C);