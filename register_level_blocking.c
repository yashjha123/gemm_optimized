// This code assumes that N is perfectly divisibly by all the constants MR and NR (i.e 4) and KC (256)

#include "include/constants.h" // constants define the N
#include "include/utils.h" // constants define the N

#include "include/optimized_gemm.h"

// we divide the matrix multiplication operation into blocks
#define block_size 512 // also refered to as b in README.m



// we want the tiles to fit in shape for registers for the micro-kernel


// hinting the compiler to inline this call

#define MR 4
#define NR 4
#define KC 256

static inline void microkernel_4x4(const int* micro_A, const int* micro_B, int* C){
    int acc[MR][NR] = {};

    for(int k = 0; k < KC; k++){
        int b[NR];
        for(int j = 0; j < NR; j++){
            b[j] = micro_B[k*NR + j];
        }

        for(int i = 0; i < MR; i++){
            const int a = micro_A[k*MR + i];

            for(int j = 0; j < NR; j++){
                acc[i][j] += a * b[j];
            }
        }
    }

    for(int i = 0; i < MR; i++){
        int *c_row = C + i * N;
        for(int j = 0; j < NR; j++){
            c_row[j] += acc[i][j];
        }
    }
}

static inline void micro_pack_A(const int *A, const int i_offset, const int k_offset, int*  packA){
    for(int k = 0; k < KC; k++){
        const int kk = k_offset + k;
        const int base = k * MR;
        for(int i = 0; i < MR; i++){
            const int ii = i_offset + i;
            packA[base + i]  = A[index(ii, kk)];
        }
    }
}

static inline void micro_pack_B(const int *A, const int k_offset, const int j_offset, int*  packB){
    for(int k = 0; k < KC; k++){
        const int kk = k_offset + k;
        const int base = k * NR;
        for(int j = 0; j < NR; j++){
            const int jj = j_offset + j;
            packB[base + j]  = A[index(kk, jj)];
        }
    }
}





void micro_level_matrix_multiply(int *A, int *B, int *C){
    
    int n = N / block_size; // small n (i.e. number of blocks)
    
    // now we iterate the blocks of these matrices
    for(int kk = 1; kk <= n; kk++){
    for(int ii = 1; ii <= n; ii++ ){
        for(int jj = 1; jj <= n; jj++){
            for(int k = (kk-1)*block_size; k < (kk*block_size); k+=KC){
                for(int i = (ii-1)*block_size; i < (ii*block_size); i+=MR){
                    micro_pack_A(A, i, k, packA);
                    for(int j = (jj-1)*block_size; j < (jj*block_size); j+=NR){
                            // C[index(i,j)] += A[index(i, k)] * B[index(k,j)];
                            // should be hoisted
                            int packA[MR*KC];
                            int packB[KC*NR];


                            
                            micro_pack_B(B, k, j, packB);

                            int *micro_c = &C[index(i,j)];
                            microkernel_4x4(packA, packB, micro_c);
                        }
                    }
                }
            }
        }
    }
}