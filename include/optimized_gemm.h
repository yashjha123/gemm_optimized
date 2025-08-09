#pragma once

#include <stdlib.h>
#include "constants.h"
#define index(i, j) ((i) * N + (j))

void blocked_matrix_multiply(int *A, int *B, int *C);
void optimized_matrix_multiply(int *A, int *B, int *C);