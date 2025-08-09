#include <stdio.h>
#include "constants.h"
#define index(i, j) ((i) * N + (j))

// 64-byte for AVX-512 safety
#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L
  #define ALLOC_ALIGNED(ptr, bytes) posix_memalign((void**)&(ptr), 64, (bytes))
#else
  #define ALLOC_ALIGNED(ptr, bytes) ((ptr) = aligned_alloc(64, (bytes)), ((ptr)!=NULL?0:1))
#endif


void print_2x2matrix(int *A);
