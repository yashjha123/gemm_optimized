# clang++ \
# -O3 \
# -DNDEBUG \
# -ffast-math \
# -lrt \
# -fenable-matrix \
# --target=arm-none-eabi \
# -march=armv7-a \
# microkernel.c
clang microkernel.c -fenable-matrix -o microkernel.o