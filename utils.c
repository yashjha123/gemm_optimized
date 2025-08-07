#include "include/utils.h"

void print_2x2matrix(int *A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", A[index(i, j)]);
        }
        printf("\n");
    }
    printf("\n");
}