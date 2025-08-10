// This code assumes that N is perfectly divisibly by all the constants MR and NR (i.e 4) and KC (256)

#include "include/constants.h" // constants define the N
#include "include/utils.h" // constants define the N

#include "include/optimized_gemm.h"

// we divide the matrix multiplication operation into blocks
#define block_size 256 // also refered to as b in README.m, experimentally determined
#define MC 256 // experimentally determined
#define NC 256 // experimentally determined



// we want the tiles to fit in shape for registers for the micro-kernel


// hinting the compiler to inline this call

#define MR 4
#define NR 4
#define KC 256



// The micro-kernel is the heart of the matrix multiplication algorithm. It is well-optimized
// to perform the multiplication of small blocks of matrices A and B, and accumulate the results
// into the C matrix, while the intermeddiate results be stored in the registers.
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

// static inline void microkernel_4x4(const int* restrict micro_A, const int* restrict micro_B, const int* restrict C){

//     int c00=0, c01=0, c02=0, c03=0;
//     int c10=0, c11=0, c12=0, c13=0;
//     int c20=0, c21=0, c22=0, c23=0;
//     int c30=0, c31=0, c32=0, c33=0;

//     #pragma ivdep
//     #pragma unroll 4
//     for(int k = 0; k < KC; k++){
//         const int* restrict Ak = micro_A + k*MR;
//         const int* restrict Bk = micro_B + k*NR;

//         const int a0 = Ak[0];
//         const int a1 = Ak[1];
//         const int a2 = Ak[2];
//         const int a3 = Ak[3];

//         const int b0 = Bk[0];
//         const int b1 = Bk[1];
//         const int b2 = Bk[2];
//         const int b3 = Bk[3];


//         c00 += a0 * b0; c01 += a0 * b1; c02 += a0 * b2; c03 += a0 * b3;
//         c10 += a1 * b0; c11 += a1 * b1; c12 += a1 * b2; c13 += a1 * b3;
//         c20 += a2 * b0; c21 += a2 * b1; c22 += a2 * b2; c23 += a2 * b3;
//         c30 += a3 * b0; c31 += a3 * b1; c32 += a3 * b2; c33 += a3 * b3;
//     }

//     int* restrict c_row0 = (int*)C + N*0;
//     int* restrict c_row1 = (int*)C + N*1;
//     int* restrict c_row2 = (int*)C + N*2;
//     int* restrict c_row3 = (int*)C + N*3;

//     c_row0[0] += c00; c_row0[1] += c01; c_row0[2] += c02; c_row0[3] += c03;
//     c_row1[0] += c10; c_row1[1] += c11; c_row1[2] += c12; c_row1[3] += c13;
//     c_row2[0] += c20; c_row2[1] += c21; c_row2[2] += c22; c_row2[3] += c23;
//     c_row3[0] += c30; c_row3[1] += c31; c_row3[2] += c32; c_row3[3] += c33;
// }

// micro packing creates micro-blocks of size MR X KC for the micro-kernel
// it packs a block of A and B matrices into a format that the micro-kernel can
// process efficiently. (in a linear fashion)
static inline void micro_pack_A(const int *A, const int i_offset, const int k_offset, int* restrict  packA){
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

// micro packing is the same as above but for B matrix. Same ways, it packs the blocsk
// so the micro-kernel can process it efficiently.
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

// pack huge A blocks for blocking
static inline void macro_pack_A(const int *A, const int i_offset, const int k_offset, int* restrict packA){
   for (int ib = 0; ib < block_size/MR; ib++) {
        int i0 = i_offset + ib * MR;
        int* restrict destA = packA + ib * KC * MR;
        micro_pack_A(A, i0, k_offset, destA);
    }
}

// pack B block for blocking
static inline void macro_pack_B(const int *B, const int k_offset, const int j_offset, int* restrict packB){
    for(int jb = 0; jb < block_size/NR; jb++){
        int j0 = j_offset + jb * NR;
        int* restrict destB = packB + jb * KC * NR;
        micro_pack_B(B, k_offset, j0, destB);
    }
}


void optimized_matrix_multiply(int  * restrict A, int  * restrict B, int  *restrict C){

    // Assuming the correct alignment of the arrays ensure the data is processed 
    A = (int *) __builtin_assume_aligned(A, 64);
    B = (int *) __builtin_assume_aligned(B, 64);
    C = (int *) __builtin_assume_aligned(C, 64);

    int n = N / block_size; // small n (i.e. number of blocks)
    

    int *restrict packA = NULL, *restrict packB = NULL;
    if (ALLOC_ALIGNED(packA, KC*MC*sizeof(int)) || ALLOC_ALIGNED(packB, KC*NC*sizeof(int))) {
        return;
    }



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

    // free((void*)packA);
    // free((void*)packB);
}