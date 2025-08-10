#include <stdio.h>
#include <stdlib.h>


// #include "blocked_gemm.c"
#include "include/optimized_gemm.h"
#include "include/utils.h"
#include "include/naive_gemm.h"
#include "include/constants.h"

#include <time.h>

#ifndef argsN
#define argsN 1024
#endif

int main(int argc, char *argv[]){
    printf("Starting matrix multiplication...\n");
    
    // int *A = (int *)(malloc(N*N*sizeof(int)));
    // int *B = (int *)(malloc(N*N*sizeof(int)));
    // int *C = (int *)(malloc(N*N*sizeof(int)));
    N = argsN;
    int *A, *B, *C;
    ALLOC_ALIGNED(A, N*N*sizeof(int));
    ALLOC_ALIGNED(B, N*N*sizeof(int));
    ALLOC_ALIGNED(C, N*N*sizeof(int));
    // alloc_aligned_64(&A, N * N * sizeof(int));
    // alloc_aligned_64(&B, N * N * sizeof(int));
    // alloc_aligned_64(&C, N * N * sizeof(int));
    
    // int *ref = (int *)(malloc(N*N*sizeof(int)));
    
    // int A[N*N] = {0};
    // int B[N*N] = {0};
    // int C[N*N] = {0};

    for(int i = 0; i < N*N; i++){
        A[i] = i;
        B[i] = 1;
        C[i] = 0;
    }

    clock_t start, end;


    start = clock();

    // #ifdef naive
        // naive_matrix_multiply(A, B, C);
    // #else
    //     optimized_matrix_multiply(A, B, C);
    // #endif
    
    // matrix_multiply_k_i_j(A, B, ref);


    // matrix_multiply_j_k_i(A,B,C);
    // matrix_multiply_i_k_j(A,B,C);
    // matrix_multiply_k_j_i(A,B,C);
    // matrix_multiply_j_i_k(A,B,C);
    // blocked_matrix_multiply(A, B, C);
    // optimized_matrix_multiply(A, B, C);

    cache_oblivious_matrix_multiply(A, B, C);
    end = clock();

    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;


    

    // print_2x2matrix(A);
    // print_2x2matrix(B);
    // print_2x2matrix(C);

    printf("Multiplication done...\n");
    printf("Computation took %.2f seconds...\n", cpu_time_used);
    return 0;
}