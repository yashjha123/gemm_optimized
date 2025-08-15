# clang \
# -O3 \
# -mcpu=alderlake \
# -fenable-matrix \
# -S microkernel.c \
# -o kernel.s
clang -O3 -S microkernel.c -o f32_kernel.s -fenable-matrix
llvm-mca -mcpu=alderlake -timeline -all-stats f32_kernel.s
