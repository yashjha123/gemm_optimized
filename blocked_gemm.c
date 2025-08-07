#include "include/constants.h" // constants define the N
#include "include/utils.h" // constants define the N


// we divide the matrix multiplication operation into blocks


#define block_size 1024 // also refered to as b in README.md

void blocked_matrix_multiply(int *A, int *B, int *C){
    
    int n = N / block_size; // small n (i.e. number of blocks)
    
    // now we iterate the blocks of these matrices
    for(int kk = 1; kk <= n; kk++){
    for(int ii = 1; ii <= n; ii++ ){
        for(int jj = 1; jj <= n; jj++){
            for(int k = (kk-1)*block_size; k < (kk*block_size); k++){
                for(int i = (ii-1)*block_size; i < (ii*block_size); i++){
                    for(int j = (jj-1)*block_size; j < (jj*block_size); j++){
                            C[index(i,j)] += A[index(i, k)] * B[index(k,j)];
                        }
                    }
                }
            }
        }
    }
}