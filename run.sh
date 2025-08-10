clang++ bench_gemm.cc -x c  constants_runtime.c \
    -x c lvl3_optimized_gemm.c -x c  utils.c  \
    -x c lvl0_naive_gemm.c \
    -x c lvl4_cache_oblivious.c \
  -Iinclude -O3 -DNDEBUG -march=native -mfma -ffast-math -mavx2 -funroll-loops \
  -D_POSIX_C_SOURCE=200112L \
  -isystem benchmark/include \
  -Lbenchmark/build/src -lbenchmark \
  -lbenchmark -lpthread -lrt \
  -o bench_gemm


./bench_gemm --benchmark_repetitions=100 --benchmark_format=json --benchmark_out_format=json --benchmark_out=bench_f.json 

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


mkdir -p perf_results

NS=(256 512 1024 2048)
for N in "${NS[@]}"; do

  clang main.c constants_runtime.c  \
      lvl3_optimized_gemm.c utils.c \
      lvl0_naive_gemm.c \
      -std=gnu11 -Iinclude -O3 -DNDEBUG \
      -march=native -mfma -ffast-math -mavx2 -funroll-loops \
      -D_POSIX_C_SOURCE=200112L \
      -DargsN=$N \
      -Dnaive=1 \
      -o matmul  

  perf stat -e cache-misses,LLC-load-misses,LLC-store-misses \
      -o perf_results/$N.out.naive.txt \
      ./matmul 


  clang main.c constants_runtime.c  \
      lvl3_optimized_gemm.c utils.c \
      lvl0_naive_gemm.c \
      -std=gnu11 -Iinclude -O3 -DNDEBUG \
      -march=native -mfma -ffast-math -mavx2 -funroll-loops \
      -D_POSIX_C_SOURCE=200112L \
      -DargsN=$N \
      -o matmul  

  perf stat -e cache-misses,LLC-load-misses,LLC-store-misses \
      -o perf_results/$N.out.optimized.txt \
      ./matmul 



  clang main.c constants_runtime.c  \
      lvl3_optimized_gemm.c utils.c \
      lvl0_naive_gemm.c \
      lvl4_cache_oblivious.c \
      -std=gnu11 -Iinclude -O3 -DNDEBUG \
      -march=native -mfma -ffast-math -mavx2 -funroll-loops \
      -D_POSIX_C_SOURCE=200112L \
      -DargsN=$N \
      -o matmul  

  perf stat -e cache-misses,LLC-load-misses,LLC-store-misses \
      -o perf_results/$N.out.cache_oblivious.txt \
      ./matmul 

done

clang main.c constants_runtime.c  \
      lvl3_optimized_gemm.c utils.c \
      lvl0_naive_gemm.c \
      lvl4_cache_oblivious.c \
      -std=gnu11 -Iinclude -O3 -DNDEBUG \
      -march=native -mfma -ffast-math -mavx2 -funroll-loops \
      -D_POSIX_C_SOURCE=200112L \
      -DargsN=2048 \
      -o matmul  

./matmul

./bench_gemm --benchmark_repetitions=100 --benchmark_format=json --benchmark_out_format=json --benchmark_out=bench_cache_oblivious.json