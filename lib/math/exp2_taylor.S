# exp2 taylor4

	.globl	exp2_taylor
	.type	exp2_taylor,@function
	.align	16
exp2_taylor:
	#rdi -- pointer to dst array
	#rsi -- pointer to src array
	#rdx -- length of the array (number of floats)

	xor %r11, %r11
	mov %rdx, %r10
	shr $0x5, %rdx

1:	je 2f

	vmovaps 0x0(%rsi, %r11, 4), %ymm0
	vmovaps 0x20(%rsi, %r11, 4), %ymm3
	vmovaps 0x40(%rsi, %r11, 4), %ymm6
	vmovaps 0x60(%rsi, %r11, 4), %ymm9
	vminps max_val(%rip), %ymm0, %ymm0
	vminps max_val(%rip), %ymm3, %ymm3
	vminps max_val(%rip), %ymm6, %ymm6
	vminps max_val(%rip), %ymm9, %ymm9
	vmaxps min_val(%rip), %ymm0, %ymm0
	vmaxps min_val(%rip), %ymm3, %ymm3
	vmaxps min_val(%rip), %ymm6, %ymm6
	vmaxps min_val(%rip), %ymm9, %ymm9
	vcvtps2dq %ymm0, %ymm2
	vcvtps2dq %ymm3, %ymm5
	vcvtps2dq %ymm6, %ymm8
	vcvtps2dq %ymm9, %ymm11
	vcvtdq2ps %ymm2, %ymm1
	vcvtdq2ps %ymm5, %ymm4
	vcvtdq2ps %ymm8, %ymm7
	vcvtdq2ps %ymm11, %ymm10
	vsubps %ymm1, %ymm0, %ymm1
	vsubps %ymm4, %ymm3, %ymm4
	vsubps %ymm7, %ymm6, %ymm7
	vsubps %ymm10, %ymm9, %ymm10

	vpaddd mantissa_add(%rip), %ymm2, %ymm2
	vpaddd mantissa_add(%rip), %ymm5, %ymm5
	vpaddd mantissa_add(%rip), %ymm8, %ymm8
	vpaddd mantissa_add(%rip), %ymm11, %ymm11
	vpslld $23, %ymm2, %ymm0
	vpslld $23, %ymm5, %ymm3
	vpslld $23, %ymm8, %ymm6
	vpslld $23, %ymm11, %ymm9

	vmovaps L4(%rip), %ymm2 # L4 -> %ymm2
	vmovaps L4(%rip), %ymm5 # L4 -> %ymm5
	vmovaps L4(%rip), %ymm8 # L4 -> %ymm8
	vmovaps L4(%rip), %ymm11 # L4 -> %ymm11
	
	vfmadd213ps L3(%rip), %ymm1, %ymm2
	vfmadd213ps L3(%rip), %ymm4, %ymm5
	vfmadd213ps L3(%rip), %ymm7, %ymm8
	vfmadd213ps L3(%rip), %ymm10, %ymm11
	vfmadd213ps L2(%rip), %ymm1, %ymm2
	vfmadd213ps L2(%rip), %ymm4, %ymm5
	vfmadd213ps L2(%rip), %ymm7, %ymm8
	vfmadd213ps L2(%rip), %ymm10, %ymm11
	vfmadd213ps L1(%rip), %ymm1, %ymm2
	vfmadd213ps L1(%rip), %ymm4, %ymm5
	vfmadd213ps L1(%rip), %ymm7, %ymm8
	vfmadd213ps L1(%rip), %ymm10, %ymm11
	vfmadd213ps one(%rip), %ymm1, %ymm2
	vfmadd213ps one(%rip), %ymm4, %ymm5
	vfmadd213ps one(%rip), %ymm7, %ymm8
	vfmadd213ps one(%rip), %ymm10, %ymm11

	vmulps %ymm0, %ymm2, %ymm0
	vmulps %ymm3, %ymm5, %ymm3
	vmulps %ymm6, %ymm8, %ymm6
	vmulps %ymm9, %ymm11, %ymm9
	vmovaps %ymm0, 0x0(%rdi, %r11, 4)
	vmovaps %ymm3, 0x20(%rdi, %r11, 4)
	vmovaps %ymm6, 0x40(%rdi, %r11, 4)
	vmovaps %ymm9, 0x60(%rdi, %r11, 4)

	lea 0x20(%r11), %r11
	dec %rdx
	jmp 1b

2: 	mov %r10, %rdx
	and $0x1F, %rdx
	shr $0x3, %rdx

1:	je 2f
	vmovaps 0x0(%rsi, %r11, 4), %ymm0
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
	vmovaps %ymm0, 0x0(%rdi, %r11, 4)

	lea 0x8(%r11), %r11
	dec %rdx
	jmp 1b

2:	ret


	.globl	exp2_taylor_one
	.type	exp2_taylor_one,@function
	.align	16
exp2_taylor_one:
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

	ret


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

