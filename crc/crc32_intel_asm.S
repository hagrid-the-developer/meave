	.globl	crc32_intel_asm
	.type	crc32_intel_asm,@function
	.align	16
crc32_intel_asm:
	# rdi -- pointer to an array
	# rsi -- length of the array
	# rdx -- value of the checksum
	mov %rsi, %r11
	shr $0x3, %r11
1:	je 2f
	crc32q (%rdi), %rdx
	lea 0x8(%rdi), %rdi
	dec %r11
	jmp 1b
2:	mov %rsi, %r11 # if (%rsi & $0x4)
	and $0x4, %r11
	je 3f
	crc32b 0x0(%rdi), %rdx
	crc32b 0x1(%rdi), %rdx
	crc32b 0x2(%rdi), %rdx
	crc32b 0x3(%rdi), %rdx
	lea 0x4(%rdi), %rdi
3:	mov %rsi, %r11 # if (%rsi & $0x2)
	and $0x2, %r11
	je 4f
	crc32b 0x0(%rdi), %rdx
	crc32b 0x1(%rdi), %rdx
	lea 0x2(%rdi), %rdi
4:	mov %rsi, %r11 # if (%rsi & $0x1)
	and $0x1, %r11
	je 5f
	crc32b (%rdi), %rdx
5:	mov %rdx, %rax # return value
	ret


	.globl	crc32_threesome_kernel
	.type	crc32_threesome_kernel,@function
	.align	16
crc32_threesome_kernel:
	# rdi -- pointer to an array
	# rsi -- number of 24bytes chunks in the array
	# rdx -- value of the checksum
	# rcx -- rsi * 8

	# ASSERT(%rsi % 8 == 0)
	# ASSERT(%rsi > 8)

	xor %r8, %r8 # %r8 ... CRC of the second part
	xor %r9, %r9 # %r9 ... CRC of the third part

	mov %rsi, %r11 # length/24 -> %r11
	dec %r11 # --%r11
1:	je 2f
	crc32q (%rdi), %rdx
	crc32q (%rdi, %rcx, 1), %r8
	crc32q (%rdi, %rcx, 2), %r9
	lea 0x8(%rdi), %rdi
	dec %r11
	jmp 1b

2:	# CRC of the last part will be calculated during combination of the parts
	crc32q (%rdi), %rdx
	crc32q (%rdi, %rcx, 1), %r8

	# Combine all three values
	# http://www.intel.com/content/dam/www/public/us/en/documents/white-papers/crc-iscsi-polynomial-crc32-instruction-paper.pdf
	#    Page 40.
	lea K_table(%rip), %r11 # first entry is for idx 1
	movdqa -0x10(%r11, %rcx, 2), %xmm0 # 2 consts: K1:K2

	# http://www.felixcloutier.com/x86/PCLMULQDQ%20-%20Carry-Less%20Multiplication%20Quadword.html
	# FIXME: %r8, %r9 and other registers are used in a way from the article, but not according to this code.
	movq %rdx, %xmm1 # CRC for block 1
	pclmulqdq $0x00, %xmm0, %xmm1 # Multiply by K2
	movq %r8, %xmm2 # CRC for block 2
	pclmulqdq $0x10, %xmm0, %xmm2 # Multiply by K1
	pxor %xmm2, %xmm1
	movq %xmm1, %rax
	xor (%rdi, %rcx, 2), %rax

	# FIXME: This is still wrong!
	crc32 %rax, %r9
	mov %r9, %rax # return value
	ret


# Number constants in GNU ASM: http://web.mit.edu/gnu/doc/html/as_7.html .
	.align 64
K_table:
.quad 0x14cd00bd6, 0x105ec76f0 #001
.quad 0x0ba4fc28e, 0x14cd00bd6 #002
.quad 0x1d82c63da, 0x0f20c0dfe #003
.quad 0x09e4addf8, 0x0ba4fc28e #004
.quad 0x039d3b296, 0x1384aa63a #005
.quad 0x102f9b8a2, 0x1d82c63da #006
.quad 0x14237f5e6, 0x01c291d04 #007
.quad 0x00d3b6092, 0x09e4addf8 #008
.quad 0x0c96cfdc0, 0x0740eef02 #009
.quad 0x18266e456, 0x039d3b296 #010
.quad 0x0daece73e, 0x0083a6eec #011
.quad 0x0ab7aff2a, 0x102f9b8a2 #012
.quad 0x1248ea574, 0x1c1733996 #013
.quad 0x083348832, 0x14237f5e6 #014
.quad 0x12c743124, 0x02ad91c30 #015
.quad 0x0b9e02b86, 0x00d3b6092 #016
.quad 0x018b33a4e, 0x06992cea2 #017
.quad 0x1b331e26a, 0x0c96cfdc0 #018
.quad 0x17d35ba46, 0x07e908048 #019
.quad 0x1bf2e8b8a, 0x18266e456 #020
.quad 0x1a3e0968a, 0x11ed1f9d8 #021
.quad 0x0ce7f39f4, 0x0daece73e #022
.quad 0x061d82e56, 0x0f1d0f55e #023
.quad 0x0d270f1a2, 0x0ab7aff2a #024
.quad 0x1c3f5f66c, 0x0a87ab8a8 #025
.quad 0x12ed0daac, 0x1248ea574 #026
.quad 0x065863b64, 0x08462d800 #027
.quad 0x11eef4f8e, 0x083348832 #028
.quad 0x1ee54f54c, 0x071d111a8 #029
.quad 0x0b3e32c28, 0x12c743124 #030
.quad 0x0064f7f26, 0x0ffd852c6 #031
.quad 0x0dd7e3b0c, 0x0b9e02b86 #032
.quad 0x0f285651c, 0x0dcb17aa4 #033
.quad 0x010746f3c, 0x018b33a4e #034
.quad 0x1c24afea4, 0x0f37c5aee #035
.quad 0x0271d9844, 0x1b331e26a #036
.quad 0x08e766a0c, 0x06051d5a2 #037
.quad 0x093a5f730, 0x17d35ba46 #038
.quad 0x06cb08e5c, 0x11d5ca20e #039
.quad 0x06b749fb2, 0x1bf2e8b8a #040
.quad 0x1167f94f2, 0x021f3d99c #041
.quad 0x0cec3662e, 0x1a3e0968a #042
.quad 0x19329634a, 0x08f158014 #043
.quad 0x0e6fc4e6a, 0x0ce7f39f4 #044
.quad 0x08227bb8a, 0x1a5e82106 #045
.quad 0x0b0cd4768, 0x061d82e56 #046
.quad 0x13c2b89c4, 0x188815ab2 #047
.quad 0x0d7a4825c, 0x0d270f1a2 #048
.quad 0x10f5ff2ba, 0x105405f3e #049
.quad 0x00167d312, 0x1c3f5f66c #050
.quad 0x0f6076544, 0x0e9adf796 #051
.quad 0x026f6a60a, 0x12ed0daac #052
.quad 0x1a2adb74e, 0x096638b34 #053
.quad 0x19d34af3a, 0x065863b64 #054
.quad 0x049c3cc9c, 0x1e50585a0 #055
.quad 0x068bce87a, 0x11eef4f8e #056
.quad 0x1524fa6c6, 0x19f1c69dc #057
.quad 0x16cba8aca, 0x1ee54f54c #058
.quad 0x042d98888, 0x12913343e #059
.quad 0x1329d9f7e, 0x0b3e32c28 #060
.quad 0x1b1c69528, 0x088f25a3a #061
.quad 0x02178513a, 0x0064f7f26 #062
.quad 0x0e0ac139e, 0x04e36f0b0 #063
.quad 0x0170076fa, 0x0dd7e3b0c #064
.quad 0x141a1a2e2, 0x0bd6f81f8 #065
.quad 0x16ad828b4, 0x0f285651c #066
.quad 0x041d17b64, 0x19425cbba #067
.quad 0x1fae1cc66, 0x010746f3c #068
.quad 0x1a75b4b00, 0x18db37e8a #069
.quad 0x0f872e54c, 0x1c24afea4 #070
.quad 0x01e41e9fc, 0x04c144932 #071
.quad 0x086d8e4d2, 0x0271d9844 #072
.quad 0x160f7af7a, 0x052148f02 #073
.quad 0x05bb8f1bc, 0x08e766a0c #074
.quad 0x0a90fd27a, 0x0a3c6f37a #075
.quad 0x0b3af077a, 0x093a5f730 #076
.quad 0x04984d782, 0x1d22c238e #077
.quad 0x0ca6ef3ac, 0x06cb08e5c #078
.quad 0x0234e0b26, 0x063ded06a #079
.quad 0x1d88abd4a, 0x06b749fb2 #080
.quad 0x04597456a, 0x04d56973c #081
.quad 0x0e9e28eb4, 0x1167f94f2 #082
.quad 0x07b3ff57a, 0x19385bf2e #083
.quad 0x0c9c8b782, 0x0cec3662e #084
.quad 0x13a9cba9e, 0x0e417f38a #085
.quad 0x093e106a4, 0x19329634a #086
.quad 0x167001a9c, 0x14e727980 #087
.quad 0x1ddffc5d4, 0x0e6fc4e6a #088
.quad 0x00df04680, 0x0d104b8fc #089
.quad 0x02342001e, 0x08227bb8a #090
.quad 0x00a2a8d7e, 0x05b397730 #091
.quad 0x168763fa6, 0x0b0cd4768 #092
.quad 0x1ed5a407a, 0x0e78eb416 #093
.quad 0x0d2c3ed1a, 0x13c2b89c4 #094
.quad 0x0995a5724, 0x1641378f0 #095
.quad 0x19b1afbc4, 0x0d7a4825c #096
.quad 0x109ffedc0, 0x08d96551c #097
.quad 0x0f2271e60, 0x10f5ff2ba #098
.quad 0x00b0bf8ca, 0x00bf80dd2 #099
.quad 0x123888b7a, 0x00167d312 #100
.quad 0x1e888f7dc, 0x18dcddd1c #101
.quad 0x002ee03b2, 0x0f6076544 #102
.quad 0x183e8d8fe, 0x06a45d2b2 #103
.quad 0x133d7a042, 0x026f6a60a #104
.quad 0x116b0f50c, 0x1dd3e10e8 #105
.quad 0x05fabe670, 0x1a2adb74e #106
.quad 0x130004488, 0x0de87806c #107
.quad 0x000bcf5f6, 0x19d34af3a #108
.quad 0x18f0c7078, 0x014338754 #109
.quad 0x017f27698, 0x049c3cc9c #110
.quad 0x058ca5f00, 0x15e3e77ee #111
.quad 0x1af900c24, 0x068bce87a #112
.quad 0x0b5cfca28, 0x0dd07448e #113
.quad 0x0ded288f8, 0x1524fa6c6 #114
.quad 0x059f229bc, 0x1d8048348 #115
.quad 0x06d390dec, 0x16cba8aca #116
.quad 0x037170390, 0x0a3e3e02c #117
.quad 0x06353c1cc, 0x042d98888 #118
.quad 0x0c4584f5c, 0x0d73c7bea #119
.quad 0x1f16a3418, 0x1329d9f7e #120
.quad 0x0531377e2, 0x185137662 #121
.quad 0x1d8d9ca7c, 0x1b1c69528 #122
.quad 0x0b25b29f2, 0x18a08b5bc #123
.quad 0x19fb2a8b0, 0x02178513a #124
.quad 0x1a08fe6ac, 0x1da758ae0 #125
.quad 0x045cddf4e, 0x0e0ac139e #126
.quad 0x1a91647f2, 0x169cf9eb0 #127
.quad 0x1a0f717c4, 0x0170076fa #128
