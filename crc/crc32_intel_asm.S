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
	# rsi -- length of the (one third) of the array
	# rdx -- value of the checksum

	# ASSERT(%rsi % 8 == 0)
	# ASSERT(%rsi > 8)

	xor %r8, %r8 # %r8 ... CRC of the second part
	xor %r9, %r9 # %r9 ... CRC of the third part

	mov %rsi, %r11 # length -> %r11
	shr $0x3, %r11 # %r11 / 8
	mov %r11, %rax
	dec %r11 # --%r11
1:	je 2f
	crc32q (%rdi), %rdx
	crc32q (%rdi, %rsi, 1), %r8
	crc32q (%rdi, %rsi, 2), %r9
	lea 0x8(%rdi), %rdi
	dec %r11
	jmp 1b

2:	# CRC of the last part will be calculated during combination of the parts
	crc32q (%rdi), %rdx
	crc32q (%rdi, %rsi, 1), %r8

	# Combine all three values
	# http://www.intel.com/content/dam/www/public/us/en/documents/white-papers/crc-iscsi-polynomial-crc32-instruction-paper.pdf
	#    Page 40.
	lea K_table(%rip), %rcx # first entry is for idx 1
	movdqa -0x10(%rcx, %rax, 1), %xmm0 # 2 consts: K1:K2

	# http://www.felixcloutier.com/x86/PCLMULQDQ%20-%20Carry-Less%20Multiplication%20Quadword.html
	movq %r8, %xmm1 # CRC for block 1
	pclmulqdq 0x00, %xmm0, %xmm1 # Multiply by K2
	movq %r9, %xmm2 # CRC for block 2
	pclmulqdq 0x01, %xmm0, %xmm2 # Multiply by K1
	pxor %xmm2, %xmm1
	movq %xmm1, %rax
	xor (%rdi, %rsi, 2), %rax

	mov %r10, %r8
	crc32 %rax, %r8
	mov %r8, %rax # return value
	ret


K_table:
dq 0x0ba4fc28e, 0x14cd00bd6
dq 0x1d82c63da, 0x0f20c0dfe
dq 0x09e4addf8, 0x0ba4fc28e
dq 0x039d3b296, 0x1384aa63a
dq 0x102f9b8a2, 0x1d82c63da
dq 0x14237f5e6, 0x01c291d04
dq 0x00d3b6092, 0x09e4addf8
dq 0x0c96cfdc0, 0x0740eef02
dq 0x18266e456, 0x039d3b296
dq 0x0daece73e, 0x0083a6eec
dq 0x0ab7aff2a, 0x102f9b8a2
dq 0x1248ea574, 0x1c1733996
dq 0x083348832, 0x14237f5e6
dq 0x12c743124, 0x02ad91c30
dq 0x0b9e02b86, 0x00d3b6092
dq 0x018b33a4e, 0x06992cea2
dq 0x1b331e26a, 0x0c96cfdc0
dq 0x17d35ba46, 0x07e908048
dq 0x1bf2e8b8a, 0x18266e456
dq 0x1a3e0968a, 0x11ed1f9d8
dq 0x0ce7f39f4, 0x0daece73e
dq 0x061d82e56, 0x0f1d0f55e
dq 0x0d270f1a2, 0x0ab7aff2a
dq 0x1c3f5f66c, 0x0a87ab8a8
dq 0x12ed0daac, 0x1248ea574
dq 0x065863b64, 0x08462d800
dq 0x11eef4f8e, 0x083348832
dq 0x1ee54f54c, 0x071d111a8
dq 0x0b3e32c28, 0x12c743124
dq 0x0064f7f26, 0x0ffd852c6
dq 0x0dd7e3b0c, 0x0b9e02b86
dq 0x0f285651c, 0x0dcb17aa4
dq 0x010746f3c, 0x018b33a4e
dq 0x1c24afea4, 0x0f37c5aee
dq 0x0271d9844, 0x1b331e26a
dq 0x08e766a0c, 0x06051d5a2
dq 0x093a5f730, 0x17d35ba46
dq 0x06cb08e5c, 0x11d5ca20e
dq 0x06b749fb2, 0x1bf2e8b8a
dq 0x1167f94f2, 0x021f3d99c
dq 0x0cec3662e, 0x1a3e0968a
dq 0x19329634a, 0x08f158014
dq 0x0e6fc4e6a, 0x0ce7f39f4
dq 0x08227bb8a, 0x1a5e82106
dq 0x0b0cd4768, 0x061d82e56
dq 0x13c2b89c4, 0x188815ab2
dq 0x0d7a4825c, 0x0d270f1a2
dq 0x10f5ff2ba, 0x105405f3e
dq 0x00167d312, 0x1c3f5f66c
dq 0x0f6076544, 0x0e9adf796
dq 0x026f6a60a, 0x12ed0daac
dq 0x1a2adb74e, 0x096638b34
dq 0x19d34af3a, 0x065863b64
dq 0x049c3cc9c, 0x1e50585a0
dq 0x068bce87a, 0x11eef4f8e
dq 0x1524fa6c6, 0x19f1c69dc
dq 0x16cba8aca, 0x1ee54f54c
dq 0x042d98888, 0x12913343e
dq 0x1329d9f7e, 0x0b3e32c28
dq 0x1b1c69528, 0x088f25a3a
dq 0x02178513a, 0x0064f7f26
dq 0x0e0ac139e, 0x04e36f0b0
dq 0x0170076fa, 0x0dd7e3b0c
dq 0x141a1a2e2, 0x0bd6f81f8
dq 0x16ad828b4, 0x0f285651c
dq 0x041d17b64, 0x19425cbba
dq 0x1fae1cc66, 0x010746f3c
dq 0x1a75b4b00, 0x18db37e8a
dq 0x0f872e54c, 0x1c24afea4
dq 0x01e41e9fc, 0x04c144932
dq 0x086d8e4d2, 0x0271d9844
dq 0x160f7af7a, 0x052148f02
dq 0x05bb8f1bc, 0x08e766a0c
dq 0x0a90fd27a, 0x0a3c6f37a
dq 0x0b3af077a, 0x093a5f730
dq 0x04984d782, 0x1d22c238e
dq 0x0ca6ef3ac, 0x06cb08e5c
dq 0x0234e0b26, 0x063ded06a
dq 0x1d88abd4a, 0x06b749fb2
dq 0x04597456a, 0x04d56973c
dq 0x0e9e28eb4, 0x1167f94f2
dq 0x07b3ff57a, 0x19385bf2e
dq 0x0c9c8b782, 0x0cec3662e
dq 0x13a9cba9e, 0x0e417f38a
dq 0x093e106a4, 0x19329634a
dq 0x167001a9c, 0x14e727980
dq 0x1ddffc5d4, 0x0e6fc4e6a
dq 0x00df04680, 0x0d104b8fc
dq 0x02342001e, 0x08227bb8a
dq 0x00a2a8d7e, 0x05b397730
dq 0x168763fa6, 0x0b0cd4768
dq 0x1ed5a407a, 0x0e78eb416
dq 0x0d2c3ed1a, 0x13c2b89c4
dq 0x0995a5724, 0x1641378f0
dq 0x19b1afbc4, 0x0d7a4825c
dq 0x109ffedc0, 0x08d96551c
dq 0x0f2271e60, 0x10f5ff2ba
dq 0x00b0bf8ca, 0x00bf80dd2
dq 0x123888b7a, 0x00167d312
dq 0x1e888f7dc, 0x18dcddd1c
dq 0x002ee03b2, 0x0f6076544
dq 0x183e8d8fe, 0x06a45d2b2
dq 0x133d7a042, 0x026f6a60a
dq 0x116b0f50c, 0x1dd3e10e8
dq 0x05fabe670, 0x1a2adb74e
dq 0x130004488, 0x0de87806c
dq 0x000bcf5f6, 0x19d34af3a
dq 0x18f0c7078, 0x014338754
dq 0x017f27698, 0x049c3cc9c
dq 0x058ca5f00, 0x15e3e77ee
dq 0x1af900c24, 0x068bce87a
dq 0x0b5cfca28, 0x0dd07448e
dq 0x0ded288f8, 0x1524fa6c6
dq 0x059f229bc, 0x1d8048348
dq 0x06d390dec, 0x16cba8aca
dq 0x037170390, 0x0a3e3e02c
dq 0x06353c1cc, 0x042d98888
dq 0x0c4584f5c, 0x0d73c7bea
dq 0x1f16a3418, 0x1329d9f7e
dq 0x0531377e2, 0x185137662
dq 0x1d8d9ca7c, 0x1b1c69528
dq 0x0b25b29f2, 0x18a08b5bc
dq 0x19fb2a8b0, 0x02178513a
dq 0x1a08fe6ac, 0x1da758ae0
dq 0x045cddf4e, 0x0e0ac139e
dq 0x1a91647f2, 0x169cf9eb0
dq 0x1a0f717c4, 0x0170076fa
