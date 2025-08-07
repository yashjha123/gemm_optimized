#include "utils.h"

void print_2x2matrix(int *A, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", A[index(i, j)]);
        }
        printf("\n");
    }
    printf("\n");
}