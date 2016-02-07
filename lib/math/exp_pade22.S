# 2,2 Padé approximation
# FMA(12 + x*x) -> X
# FMA(X + 6x) -> A1
# FMA(X - 6x) -> A2
# A1 / A2 -> result; A1 * VRCPPS(A2)
# * http://software-lisc.fbk.eu/avx_mathfun/avx_mathfun.h 
# * https://en.wikipedia.org/wiki/FMA_instruction_set

# FIXME: Use broadcast...

	.globl	exp_pade22
	.type	exp_pade22,@function
	.align	16
exp_pade22:
	#rdi -- pointer to dst array
	#rsi -- pointer to src array
	#rdx -- length of the array (number of floats)

	vxorps %ymm0, %ymm0, %ymm0 # ymm0 == 0
	vmovaps plus_twelve(%rip), %ymm6
	vmovaps plus_six(%rip), %ymm1
	vmovaps minus_six(%rip), %ymm2

	xor %r11, %r11
	shr $0x3, %rdx

1:	je 2f

	vmovaps (%rsi, %r11, 4), %ymm3
	vmovaps %ymm3, %ymm4
	vmovaps %ymm3, %ymm5

	vfmadd132ps %ymm3, %ymm6, %ymm4 # %ymm3 * %ymm4 + %ymm1 -> %ymm4
	vfmadd132ps %ymm1, %ymm4, %ymm3 # %ymm1 * %ymm3 + %ymm4 -> %ymm3
	vfmadd132ps %ymm2, %ymm4, %ymm5 # %ymm2 * %ymm5 + %ymm4 -> %ymm5
	vrcpps %ymm5, %ymm5             # 1/%ymm5 -> %ymm5
	vfmadd132ps %ymm5, %ymm0, %ymm3 # %ymm5 * %ymm3 + 0 -> %ymm3

	vmovaps %ymm3, (%rdi, %r11, 4)

	lea 0x8(%r11), %r11
	dec %rdx
	jmp 1b

2: 	ret

.align 32
plus_twelve:
	.float +12.0, +12.0, +12.0, +12.0,    +12.0, +12.0, +12.0, +12.0
plus_six:
	.float +6.0, +6.0, +6.0, +6.0,    +6.0, +6.0, +6.0, +6.0
minus_six:
	.float -6.0, -6.0, -6.0, -6.0,    -6.0, -6.0, -6.0, -6.0
