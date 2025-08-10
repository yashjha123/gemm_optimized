# clang++ bench_gemm.cc -x c  constants_runtime.c \
#     -x c lvl3_optimized_gemm.c -x c  utils.c  \
#     -x c lvl0_naive_gemm.c \
#   -Iinclude -O3 -DNDEBUG -march=native -mfma -ffast-math -mavx2 -funroll-loops \
#   -D_POSIX_C_SOURCE=200112L \
#   -isystem benchmark/include \
#   -Lbenchmark/build/src -lbenchmark \
#   -lbenchmark -lpthread -lrt \
#   -o bench_gemm


# ./bench_gemm --benchmark_repetitions=100 --benchmark_format=json --benchmark_out_format=json --benchmark_out=bench_f.json 

clang++ bench_gemm.cc -x c  constants_runtime.c \
    -x c lvl3_optimized_gemm.c -x c  utils.c  \
    -x c lvl0_naive_gemm.c \
  -Iinclude -O3 -DNDEBUG -mfma -ffast-math -funroll-loops \
  -fno-slp-vectorize -disable-vectorization -fno-vectorize \
  -D_POSIX_C_SOURCE=200112L \
  -isystem benchmark/include \
  -Lbenchmark/build/src -lbenchmark \
  -lbenchmark -lpthread -lrt \
  -o bench_gemm

./bench_gemm --benchmark_repetitions=2 --benchmark_format=json --benchmark_out_format=json --benchmark_out=bench_no_vector_f.json 
