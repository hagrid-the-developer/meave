# Related documents:
# * http://www.felixcloutier.com/x86/POR.html
# * http://www.felixcloutier.com/x86/PXOR.html
# * http://www.felixcloutier.com/x86/PSRLW:PSRLD:PSRLQ.html
# * https://hjlebbink.github.io/x86doc/html/PSRLW_PSRLD_PSRLQ.html
# * http://www.felixcloutier.com/x86/MOVDQU.html
# * http://www.felixcloutier.com/x86/VEXTRACTF128.html
# * http://www.felixcloutier.com/x86/VEXTRACTI128.html
# * http://www.felixcloutier.com/x86/MOVHLPS.html
# * http://x86.renejeschke.de/html/file_module_x86_id_181.html
# * https://en.wikipedia.org/wiki/X86_calling_conventions

	.globl	rolhash_kernel_avx2
	.type	rolhash_lernel_avx2,@function
	.align	16
rolhash_kernel_avx2:
	#rdi -- pointer src array
	#rsi -- length of the array (number of bytes)
	#rdx -- rol bits

#	movq %rdx, %xmm0 # shift left -> %xmm0
#	movq $0x20, %r11
#	sub %rdx, %r11
#	movq %r11, %xmm1  # 8*sizeof(uns) - rol_bits -> %xmm1
	vxorps %ymm2, %ymm2, %ymm2 # Initialise hash value
	vxorps %ymm4, %ymm4, %ymm4 # Initialise hash value
	vxorps %ymm6, %ymm6, %ymm6 # Initialise hash value
	vxorps %ymm8, %ymm8, %ymm8 # Initialise hash value

	mov %rsi, %rdx
	shr $0x7, %rdx # length / 32 -> %rdx
	jz 2f
1:	vpslld $13, %ymm2, %ymm3
	vpslld $13, %ymm4, %ymm5
	vpslld $13, %ymm6, %ymm7
	vpslld $13, %ymm8, %ymm9
	vpsrld $29, %ymm2, %ymm2
	vpsrld $29, %ymm4, %ymm4
	vpsrld $29, %ymm6, %ymm6
	vpsrld $29, %ymm8, %ymm8
	vpor %ymm2, %ymm3, %ymm2
	vpor %ymm4, %ymm5, %ymm4
	vpor %ymm6, %ymm7, %ymm6
	vpor %ymm8, %ymm9, %ymm8
	vxorps (%rdi), %ymm2, %ymm2
	vxorps 32(%rdi), %ymm4, %ymm4
	vxorps 64(%rdi), %ymm6, %ymm6
	vxorps 96(%rdi), %ymm8, %ymm8
	lea 128(%rdi), %rdi
	dec %rdx
	jnz 1b
	# Join two 128bit vals into one 128bit val
2:	# Process remaining bytes
	#...
	movd %xmm2, %eax
	ret



	mov %rsi, %rdx
	shr $0x5, %rdx # length / 32 -> %rdx
	jz 2f
1:	vpslld $13, %ymm2, %ymm3
	vpsrld $29, %ymm2, %ymm2
	vpor %ymm2, %ymm3, %ymm2
	vxorps (%rdi), %ymm2, %ymm2
	lea 32(%rdi), %rdi
	dec %rdx
	jnz 1b
	# Join two 128bit vals into one 128bit val
2:	# Process remaining bytes
	#...
	movd %xmm2, %eax
	ret
2:	vextractf128 $1, %ymm2, %xmm3
	vxorps %xmm2, %xmm3, %xmm2

	# Join two 64bit vals to one 64bit val
	movhlps %xmm2, %xmm3
	vxorps %xmm2, %xmm3, %xmm2

	# Join two 32bit vals to one 32bit val
	vpsrlq $0x20, %xmm2, %xmm3
	vxorps %xmm2, %xmm3, %xmm2

	# Process remaining bytes
	#...
	movd %xmm2, %eax
	ret




2:	and $0x1F, %rsi # length mod 32 -> %rsi
	# Process 16bytes
	mov %rsi, %rdx
	shr $0x4, %rdx # (length mod 32) / 16 -> %rdx
	jz 3f
	vpslld $13, %xmm2, %xmm3
	vpsrld $29, %xmm2, %xmm2
	vpor %xmm2, %xmm3, %xmm2
	vxorps (%rdi), %xmm2, %xmm2
	lea 16(%rdi), %rdi
	sub $0x10, %rsi
	# Join two 64bit vals to one 64bit val
3:	movhlps %xmm2, %xmm3
	vxorps %xmm2, %xmm3, %xmm2
	# Process 8 bytes
	mov %rsi, %rdx
	shr $0x3, %rdx
	je 4f
	vpslld $13, %xmm2, %xmm3
	vpsrld $29, %xmm2, %xmm2
	vpor %xmm2, %xmm3, %xmm2
	movq (%rdi), %xmm3
	vxorps %xmm3, %xmm2, %xmm2
	lea 8(%rdi), %rdi
	sub $0x8, %rsi
	# Join two 32bit vals to one 32bit val
4:	vpsrlq $0x20, %xmm2, %xmm3
	vxorps %xmm2, %xmm3, %xmm2
	mov %rsi, %rdx
	shr $0x2, %rdx
	je 5f
	# Process 4 bytes
	vpslld $13, %xmm2, %xmm3
	vpsrld $29, %xmm2, %xmm2
	vpor %xmm2, %xmm3, %xmm2
	movd (%rdi), %xmm3
	vxorps %xmm3, %xmm2, %xmm2
	lea 4(%rdi), %rdi
	sub $0x4, %rsi
5:	# Process remaining bytes
	#...
	movd %xmm2, %eax
	ret
