# Related documents:
# * http://www.felixcloutier.com/x86/POR.html
# * http://www.felixcloutier.com/x86/PXOR.html
# * http://www.felixcloutier.com/x86/PSRLW:PSRLD:PSRLQ.html
# * http://www.felixcloutier.com/x86/MOVDQU.html
# * http://www.felixcloutier.com/x86/VEXTRACTF128.html
# * http://www.felixcloutier.com/x86/VEXTRACTI128.html
# * http://www.felixcloutier.com/x86/MOVHLPS.html
# * http://x86.renejeschke.de/html/file_module_x86_id_181.html
# * https://en.wikipedia.org/wiki/X86_calling_conventions

# rothash kernel

	.globl	rothash_kernel_avx2
	.type	rothash_lernel_avx2,@function
	.align	16
rothash_kernel_avx2:
	#rdi -- pointer src array
	#rsi -- length of the array (number of floats)
	#rdx -- rol bits


	movd %rdx, %xmm0 # shift left -> %xmm0
	mov $0x20, %r11
	sub %rdx, %r11
	movd %r11, %xmm1  # 8*sizeof(uns) - rol_bits -> %xmm1
	vpxor %ymm2, %ymm2, %ymm2 # Initialise hash value

	
	mov %rsi, %rdx
	shr $0x5, %rdx # length / 32 -> %rdx

	jz 2f
1:	vpslld %xmm0, %ymm2, %ymm3
	vpsrld %xmm1, %ymm2, %ymm2
	vpor %ymm2, %ymm3, %ymm2
	vmovups (%rdi), %ymm3
	vpxor %ymm3, %ymm2, %ymm2
	lea 32(%rdi), %rdi
	dec %rdx
	jnz 1b
	# Join two 128-vals into one 128bit val
	vextractf128 $1, %ymm2, %xmm3
	vpxor %xmm2, %xmm3, %xmm2
2:	and 0x1F, %rsi # length mod 32 -> %rsi
	mov %rsi, %rdx
	shr $0x4, %rdx # (length mon 32) / 16 -> %rdx
	jz 3f
	# Process 16bytes
	vpslld %xmm0, %xmm2, %xmm3
	vpsrld %xmm1, %xmm2, %xmm2
	vpor %xmm2, %xmm3, %xmm2
	movups (%rdi), %xmm3
	vpxor %xmm3, %xmm2, %xmm2
	# Join two 64bit vals to one 64bit val
	movlhps %xmm2, %xmm3
	vpxor %xmm2, %xmm3, %xmm2
	lea 16(%rdi), %rdi
	sub $0x10, %rsi
3:	mov %rsi, %rdx
	shr $0x3, %rdx
	je 4f
	# Process 8 bytes
	vpslld %xmm0, %xmm2, %xmm3
	vpsrld %xmm1, %xmm2, %xmm2
	vpor %xmm2, %xmm3, %xmm2
	movhps (%rdi), %xmm3
	vpxor %xmm3, %xmm2, %xmm2
	# Join two 32bit vals to one 32bit val
	vpsllq $0x20, %xmm2, %xmm3
	vpxor %xmm2, %xmm3, %xmm2
	lea 8(%rdi), %rdi
	sub $0x8, %rsi
4:	mov %rsi, %rdx
	shr $0x2, %rdx
	je 5f
	# Process 4 bytes
	vpslld %xmm0, %xmm2, %xmm3
	vpsrld %xmm1, %xmm2, %xmm2
	vpor %xmm2, %xmm3, %xmm2
	movd (%rdi), %xmm3
	vpxor %xmm3, %xmm2, %xmm2
	lea 4(%rdi), %rdi
	sub $0x4, %rsi
5:	# Process remaining bytes
	#...
	movd %xmm2, %eax
