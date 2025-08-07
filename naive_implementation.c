#include <stdio.h>
#include <cstdlib>

#include "utils.h"

#define N 128
#define index(i, j) ((i) * N + (j))

void matrix_multiply(int *A, int *B, int *C){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            for (int k = 0; k < N; k++){
                C[index(i, j)] += A[index(i, k)] * B[index(k, j)];
            }
        }
    }
}


void print_2x2matrix(int *A){
    for(int i = 0; i < N; i++){
        for(int j = 0 ; j < N; j++){
            printf("%d\t", A[index(i, j)]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[]){

    
    int *A = (int *)(malloc(N*N*sizeof(int)));
    int *B = (int *)(malloc(N*N*sizeof(int)));
    int *C = (int *)(malloc(N*N*sizeof(int)));
    // int A[N*N] = {0};
    // int B[N*N] = {0};
    // int C[N*N] = {0};

    for(int i = 0; i < N*N; i++){
        A[i] = i;
        B[i] = i;
        C[i] = 0;
    }

    matrix_multiply(A, B, C);

    // print_2x2matrix(A, N);
    // print_2x2matrix(B, N);
    // print_2x2matrix(C, N);



    printf("Hello, World!\n");
    return 0;
}