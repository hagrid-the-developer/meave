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

	vmovaps (%rsi, %r11, 4), %ymm10
	vminps max_val(%rip), %ymm10, %ymm10
	vmaxps min_val(%rip), %ymm10, %ymm10
	vcvtps2dq %ymm10, %ymm13
	vcvtdq2ps %ymm13, %ymm4
	vsubps %ymm4, %ymm10, %ymm10

	vpaddd mantissa_add(%rip), %ymm13, %ymm13
	vpslld $23, %ymm13, %ymm13

	vmovaps %ymm10, %ymm11
	
	vmulps %ymm8, %ymm10, %ymm10 # %ymm8 * %ymm10-> %ymm10    # x * L -> x (X1)
	vfmadd213ps plus_four(%rip), %ymm9, %ymm11 # %ymm9 * %ymm11 + 4.0 -> %ymm11    # x * 3L + 4 -> x (X2)
	vfmadd213ps plus_six(%rip), %ymm10, %ymm11 # %ymm10 * %ymm11 + 6.0 -> %ymm11    # X1 * X2 + 6 -> x (X3)
	vfmadd213ps plus_twelve(%rip), %ymm10, %ymm11 # %ymm10 * %ymm11 + 12.0 -> %ymm11    # X1 * X2 + 12 -> x (X4)
	vmulps %ymm10, %ymm11, %ymm11 # %ymm10 * %ymm11 -> %ymm11    # X1 * X4 -> x (X5)
	vmulps one_over_twelve(%rip), %ymm11, %ymm11 # %ymm10 * %ymm11 -> %ymm11    # (1/12) * X5 -> x (X5)

	vmovaps %ymm11, (%rdi, %r11, 4)

	lea 0x8(%r11), %r11
	dec %rdx
	jmp 1b

2: 	ret

	.align	32
plus_twelve:
	.float +12.0, +12.0, +12.0, +12.0,   +12.0, +12.0, +12.0, +12.0
plus_six:
	.float +6.0, +6.0, +6.0, +6.0,    +6.0, +6.0, +6.0, +6.0
plus_four:
	.float +4.0, +4.0, +4.0, +4.0,    +4.0, +4.0, +4.0, +4.0
plus_three:
	.float +3.0, +3.0, +3.0, +3.0,    +3.0, +3.0, +3.0, +3.0
one_over_twelve:
	.float +0.08333333333333333, +0.08333333333333333, +0.08333333333333333, +0.08333333333333333,     +0.08333333333333333, +0.08333333333333333, +0.08333333333333333, +0.08333333333333333
mantissa_add:
	.int 127, 127, 127, 127,    127, 127, 127, 127
min_val:
	.float -127.0, -127.0, -127.0, -127.0,     -127.0, -127.0, -127.0, -127.0
max_val:
	.float +128.0, +128.0, +128.0, +128.0,     +128.0, +128.0, +128.0, +128.0
ln2:
	.float 0.6931471805599453094, 0.6931471805599453094, 0.6931471805599453094, 0.6931471805599453094,    0.6931471805599453094, 0.6931471805599453094, 0.6931471805599453094, 0.6931471805599453094
ln2x3:
	.float 2.0794415416798359282516963643745297, 2.0794415416798359282516963643745297, 2.0794415416798359282516963643745297, 2.0794415416798359282516963643745297,    2.0794415416798359282516963643745297, 2.0794415416798359282516963643745297, 2.0794415416798359282516963643745297, 2.0794415416798359282516963643745297
