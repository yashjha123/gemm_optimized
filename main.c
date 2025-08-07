#include <stdio.h>
#include <cstdlib>


#include "include/utils.h"
#include "include/naive.h"
#include "include/constants.h"

#include <time.h>


int main(int argc, char *argv[]){

    
    int *A = (int *)(malloc(N*N*sizeof(int)));
    int *B = (int *)(malloc(N*N*sizeof(int)));
    int *C = (int *)(malloc(N*N*sizeof(int)));
    // int A[N*N] = {0};
    // int B[N*N] = {0};
    // int C[N*N] = {0};

    for(int i = 0; i < N*N; i++){
        A[i] = 1;
        B[i] = 1;
        C[i] = 0;
    }

    clock_t start, end;

    start = clock();

    naive_matrix_multiply(A, B, C);

    end = clock();

    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;


    

    // print_2x2matrix(A, N);
    // print_2x2matrix(B, N);
    // print_2x2matrix(C, N);

    printf("Multiplication done...\n");
    printf("Computation took %.2f seconds...\n", cpu_time_used);
    return 0;
}