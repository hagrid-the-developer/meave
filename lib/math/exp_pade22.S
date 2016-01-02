# 2,2 Padé approximation
# FMA(6 + x*x) -> X
# FMA(X + 6x) -> A1
# FMA(X - 6x) -> A2
# A1 / A2 -> result; A1 * VRCPPS(A2)
# * http://software-lisc.fbk.eu/avx_mathfun/avx_mathfun.h 
# * https://en.wikipedia.org/wiki/FMA_instruction_set

	.globl	exp_pade22
	.type	exp_pade22,@function
	.align	16
exp_pade22:

	ret

.align 32
.plus_six:  +6.0f, +6.0f, +6.0f, +6.0f,    +6.0f, +6.0f, +6.0f, +6.0f
.minus_six: -6.0f, -6.0f, -6.0f, -6.0f,    -6.0f, -6.0f, -6.0f, -6.0f
