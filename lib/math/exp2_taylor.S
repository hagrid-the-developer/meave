# exp2 taylor4

	.globl	exp2_taylor
	.type	exp2_taylor,@function
	.align	16
exp2_taylor:
	#rdi -- pointer to dst array
	#rsi -- pointer to src array
	#rdx -- length of the array (number of floats)

	xor %r11, %r11
	shr $0x3, %rdx

1:	je 2f

	vmovaps (%rsi, %r11, 4), %ymm0
	vminps max_val(%rip), %ymm0, %ymm0
	vmaxps min_val(%rip), %ymm0, %ymm0
	vcvtps2dq %ymm0, %ymm2
	vcvtdq2ps %ymm2, %ymm1
	vsubps %ymm1, %ymm0, %ymm1

	vpaddd mantissa_add(%rip), %ymm2, %ymm2
	vpslld $23, %ymm2, %ymm0

	vmovaps L4(%rip), %ymm2 # L4 -> %ymm2
	
	vfmadd213ps L3(%rip), %ymm1, %ymm2
	vfmadd213ps L2(%rip), %ymm1, %ymm2
	vfmadd213ps L1(%rip), %ymm1, %ymm2
	vfmadd213ps one(%rip), %ymm1, %ymm2

	vmulps %ymm0, %ymm2, %ymm0
	vmovaps %ymm0, (%rdi, %r11, 4)

	lea 0x8(%r11), %r11
	dec %rdx
	jmp 1b

2: 	ret

	.align	32
mantissa_add:
	.int 127, 127, 127, 127,    127, 127, 127, 127
min_val:
	.float -127.0, -127.0, -127.0, -127.0,     -127.0, -127.0, -127.0, -127.0
max_val:
	.float +128.0, +128.0, +128.0, +128.0,     +128.0, +128.0, +128.0, +128.0
one:
	.float 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
L1:
	.float 0.6931471805599453094, 0.6931471805599453094, 0.6931471805599453094, 0.6931471805599453094,    0.6931471805599453094, 0.6931471805599453094, 0.6931471805599453094, 0.6931471805599453094
L2:
	.float 0.2402265, 0.2402265, 0.2402265, 0.2402265, 0.2402265, 0.2402265, 0.2402265, 0.2402265
L3:
	.float 0.05550411, 0.05550411, 0.05550411, 0.05550411, 0.05550411, 0.05550411, 0.05550411, 0.05550411
L4:
	.float 0.009618129, 0.009618129, 0.009618129, 0.009618129, 0.009618129, 0.009618129, 0.009618129, 0.009618129

