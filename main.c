#include <stdio.h>
#include <cstdlib>


#include "include/utils.h"
#include "include/naive.h"


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

    

    naive_matrix_multiply(A, B, C);

    print_2x2matrix(A, N);
    print_2x2matrix(B, N);
    print_2x2matrix(C, N);

    printf("Multiplication done...\n");
    return 0;
}