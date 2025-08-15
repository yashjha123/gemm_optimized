	.file	"microkernel.c"
	.text
	.globl	llvm_microkernel                # -- Begin function llvm_microkernel
	.p2align	4
	.type	llvm_microkernel,@function
llvm_microkernel:                       # @llvm_microkernel
	.cfi_startproc
# %bb.0:
	movdqa	%xmm0, %xmm4
	pshufd	$238, %xmm0, %xmm6              # xmm6 = xmm0[2,3,2,3]
	pshufd	$238, %xmm1, %xmm5              # xmm5 = xmm1[2,3,2,3]
	pshuflw	$0, %xmm2, %xmm0                # xmm0 = xmm2[0,0,0,0,4,5,6,7]
	pmullw	%xmm4, %xmm0
	pshuflw	$85, %xmm2, %xmm7               # xmm7 = xmm2[1,1,1,1,4,5,6,7]
	pmullw	%xmm6, %xmm7
	paddw	%xmm0, %xmm7
	pshuflw	$170, %xmm2, %xmm8              # xmm8 = xmm2[2,2,2,2,4,5,6,7]
	pmullw	%xmm1, %xmm8
	pshuflw	$255, %xmm2, %xmm0              # xmm0 = xmm2[3,3,3,3,4,5,6,7]
	pmullw	%xmm5, %xmm0
	paddw	%xmm8, %xmm0
	paddw	%xmm7, %xmm0
	pshufhw	$0, %xmm2, %xmm7                # xmm7 = xmm2[0,1,2,3,4,4,4,4]
	pshufd	$238, %xmm7, %xmm7              # xmm7 = xmm7[2,3,2,3]
	pmullw	%xmm4, %xmm7
	pshufhw	$85, %xmm2, %xmm8               # xmm8 = xmm2[0,1,2,3,5,5,5,5]
	pshufd	$238, %xmm8, %xmm8              # xmm8 = xmm8[2,3,2,3]
	pmullw	%xmm6, %xmm8
	paddw	%xmm7, %xmm8
	pshufhw	$170, %xmm2, %xmm7              # xmm7 = xmm2[0,1,2,3,6,6,6,6]
	pshufd	$238, %xmm7, %xmm7              # xmm7 = xmm7[2,3,2,3]
	pmullw	%xmm1, %xmm7
	pshufhw	$255, %xmm2, %xmm2              # xmm2 = xmm2[0,1,2,3,7,7,7,7]
	pshufd	$238, %xmm2, %xmm2              # xmm2 = xmm2[2,3,2,3]
	pmullw	%xmm5, %xmm2
	paddw	%xmm7, %xmm2
	paddw	%xmm8, %xmm2
	punpcklqdq	%xmm2, %xmm0            # xmm0 = xmm0[0],xmm2[0]
	pshuflw	$0, %xmm3, %xmm2                # xmm2 = xmm3[0,0,0,0,4,5,6,7]
	pmullw	%xmm4, %xmm2
	pshuflw	$85, %xmm3, %xmm7               # xmm7 = xmm3[1,1,1,1,4,5,6,7]
	pmullw	%xmm6, %xmm7
	paddw	%xmm2, %xmm7
	pshuflw	$170, %xmm3, %xmm8              # xmm8 = xmm3[2,2,2,2,4,5,6,7]
	pmullw	%xmm1, %xmm8
	pshuflw	$255, %xmm3, %xmm2              # xmm2 = xmm3[3,3,3,3,4,5,6,7]
	pmullw	%xmm5, %xmm2
	paddw	%xmm8, %xmm2
	paddw	%xmm7, %xmm2
	pshufhw	$0, %xmm3, %xmm7                # xmm7 = xmm3[0,1,2,3,4,4,4,4]
	pshufd	$238, %xmm7, %xmm7              # xmm7 = xmm7[2,3,2,3]
	pmullw	%xmm4, %xmm7
	pshufhw	$85, %xmm3, %xmm4               # xmm4 = xmm3[0,1,2,3,5,5,5,5]
	pshufd	$238, %xmm4, %xmm4              # xmm4 = xmm4[2,3,2,3]
	pmullw	%xmm6, %xmm4
	paddw	%xmm7, %xmm4
	pshufhw	$170, %xmm3, %xmm6              # xmm6 = xmm3[0,1,2,3,6,6,6,6]
	pshufd	$238, %xmm6, %xmm6              # xmm6 = xmm6[2,3,2,3]
	pmullw	%xmm1, %xmm6
	pshufhw	$255, %xmm3, %xmm1              # xmm1 = xmm3[0,1,2,3,7,7,7,7]
	pshufd	$238, %xmm1, %xmm1              # xmm1 = xmm1[2,3,2,3]
	pmullw	%xmm5, %xmm1
	paddw	%xmm6, %xmm1
	paddw	%xmm4, %xmm1
	punpcklqdq	%xmm1, %xmm2            # xmm2 = xmm2[0],xmm1[0]
	movdqa	%xmm2, %xmm1
	retq
.Lfunc_end0:
	.size	llvm_microkernel, .Lfunc_end0-llvm_microkernel
	.cfi_endproc
                                        # -- End function
	.ident	"Ubuntu clang version 22.0.0 (++20250812045236+544562ebc2bc-1~exp1~20250812165401.2598)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
