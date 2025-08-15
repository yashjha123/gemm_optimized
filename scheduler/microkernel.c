#include <stdint.h> 
#include <stddef.h> 

// typedef unsigned int m4x4_i32 __attribute__((matrix_type(4,4)));
typedef uint16_t m4x4_i32A __attribute__((matrix_type(1,2)));
typedef uint16_t m4x4_i32B __attribute__((matrix_type(2,1)));
typedef uint16_t m4x4_i32C __attribute__((matrix_type(1,1)));
// typedef int16_t m4x4_i8 __attribute__((matrix_type(4,4)));

typedef float m4x4_f32 __attribute__((matrix_type(2,2)));
typedef float m4x4_f32A __attribute__((matrix_type(30,30)));
typedef float m4x4_f32B __attribute__((matrix_type(30,30)));
typedef float m4x4_f32C __attribute__((matrix_type(30,30)));


// m4x4_i32C llvm_microkernel(const m4x4_i32A A, const m4x4_i32B B){
//     return A * B;
// }

m4x4_f32C llvm_microkernel_f32(const m4x4_f32A A, const m4x4_f32B B){
    return A * B;
}
