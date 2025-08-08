// This code assumes that N is perfectly divisibly by all the constants MR and NR (i.e 4) and KC (256)

#include "include/constants.h" // constants define the N
#include "include/utils.h" // constants define the N

#include "include/optimized_gemm.h"

// we divide the matrix multiplication operation into blocks
#define block_size 512 // also refered to as b in README.m
#define MC 512 // number of rows in the micro-kernel
#define NC 512 // number of rows in the micro-kernel


// we want the tiles to fit in shape for registers for the micro-kernel


// hinting the compiler to inline this call

#define MR 4
#define NR 4
#define KC 256

static inline void microkernel_4x4(const int* micro_A, const int* micro_B, int* C){
    int acc[MR][NR] = {};

    for(int k = 0; k < KC; k++){
        int b[NR];
        #pragma unroll
        for(int j = 0; j < NR; j++){
            b[j] = micro_B[k*NR + j];
        }

        #pragma unroll
        for(int i = 0; i < MR; i++){
            const int a = micro_A[k*MR + i];

            #pragma unroll
            for(int j = 0; j < NR; j++){
                acc[i][j] += a * b[j];
            }
        }
    }

    for(int i = 0; i < MR; i++){
        int *c_row = C + i * N;
        #pragma unroll
        for(int j = 0; j < NR; j++){
            c_row[j] += acc[i][j];
        }
    }
}

static inline void micro_pack_A(const int *A, const int i_offset, const int k_offset, int*restrict  packA){
    for(int k = 0; k < KC; k++){
        const int kk = k_offset + k;
        const int base = k * MR;
        #pragma unroll
        for(int i = 0; i < MR; i++){
            const int ii = i_offset + i;
            packA[base + i]  = A[index(ii, kk)];
        }
    }
}

static inline void micro_pack_B(const int *B, const int k_offset, const int j_offset, int* restrict  packB){
    for(int k = 0; k < KC; k++){
        const int kk = k_offset + k;
        const int base = k * NR;
        #pragma unroll
        for(int j = 0; j < NR; j++){
            const int jj = j_offset + j;
            packB[base + j]  = B[index(kk, jj)];
        }
    }
}


static inline void macro_pack_A(const int *A, const int i_offset, const int k_offset, int*  packA){
   for (int ib = 0; ib < block_size/MR; ib++) {
        int i0 = i_offset + ib * MR;
        int* restrict destA = packA + ib * KC * MR;
        micro_pack_A(A, i0, KC, destA);
    }
}

static inline void macro_pack_B(const int *B, const int k_offset, const int j_offset, int*  packB){
    for(int jb = 0; jb < block_size/NR; jb++){
        int j0 = j_offset + jb * NR;
        int* restrict destB = packB + jb * KC * NR;
        micro_pack_B(B, k_offset, j0, destB);
    }
}


void micro_level_matrix_multiply(int *A, int *B, int *C){
    
    A = (int *) __builtin_assume_aligned(A, 64);
    B = (int *) __builtin_assume_aligned(B, 64);
    C = (int *) __builtin_assume_aligned(C, 64);

    int n = N / block_size; // small n (i.e. number of blocks)
    

    // int packA[MR*KC*block_size];
    // int packB[KC*NR*block_size];

    int *packA = (int *) __builtin_alloca(MR * KC * block_size * sizeof(int));
    int *packB = (int *) __builtin_alloca(KC * NR * block_size * sizeof(int));

    // for (int j = 0; j < N; j += NR) {
    //     for (int k = 0; k < N; k += KC) {
    //         micro_pack_B(B, k, j, packB);
    //         for (int i = 0; i < N; i += MR) {
    //             micro_pack_A(A, i, k, packA);

    //             int *micro_c = &C[index(i, j)];
    //             microkernel_4x4(packA, packB, micro_c);
    //         }
    //     }
    // }
    for (int j0 = 0; j0 < N; j0 += NC){
        for (int k0 = 0; k0 < N; k0 += KC){
            macro_pack_B(B, k0, j0, packB);
            for (int i0 = 0; i0 < N; i0 += MC){
                macro_pack_A(A, i0, k0, packA);

                for (int ib = 0; ib < MC; ib += MR){
                    const int *restrict A_panel = packA + (ib / MR) * (KC * MR);

                    for (int jb = 0; jb < NC; jb += NR){
                        const int *restrict B_panel = packB + (jb / NR) * (KC * NR);

                        int *restrict C_tile = &C[index(i0 + ib, j0 + jb)];
                        microkernel_4x4(A_panel, B_panel, C_tile);
                    }
                }
            }
        }
    }
    // for(int jj = 0; jj <= n; jj+=block_size){
    //     for(int kk = 0; kk <= n; kk+=block_size){
    //         macro_pack_B(B, (jj-1)*block_size, (kk-1)*block_size, packB);
    //         for(int ii = 0; ii <= n; ii+=block_size){
    //             for(int j = 0; j < (jj*block_size); j+=NR){
    //                 for(int k = (kk-1)*block_size; k < (kk*block_size); k+=KC){
    //                     micro_pack_B(B, k, j, packB);
    //                     for(int i = (ii-1)*block_size; i < (ii*block_size); i+=MR){
    //                         micro_pack_A(A, i, k, packA);
        
        
    //     // now we iterate the blocks of these matrices

    //                         // C[index(i,j)] += A[index(i, k)] * B[index(k,j)];
    //                         // should be hoisted


                            

    //                         int *micro_c = &C[index(i,j)];
    //                         microkernel_4x4(packA, packB, micro_c);
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
}