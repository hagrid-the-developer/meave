# Related documents:
# * http://www.felixcloutier.com/x86/POR.html
# * http://www.felixcloutier.com/x86/PXOR.html
# * http://www.felixcloutier.com/x86/PSRLW:PSRLD:PSRLQ.html
# * https://en.wikipedia.org/wiki/X86_calling_conventions

# rothash kernel

	.globl	rothash_kernel_avx2
	.type	rothash_lernel_avx2,@function
	.align	16
rothash_kernel_avx2:
	#rdi -- pointer src array
	#rsi -- length of the array (number of floats)
	#rdx -- rol bits


	mov %rdx, %xmm0 # shift left -> %xmm0
	mov $0x20, %r11
	sub %rdx. %r11
	mov %r11, %xmm1  # 8*sizeof(uns) - rol_bits -> %xmm1
	vpxor %ymm2, %ymm2, %ymm2 # Initialise hash value

	
	mov %rsi, %rdx
	shr $0x5, %rdx # length / 32 -> %rdx

1:	je 2f
	vpslld %xmm0, %ymm2, %ymm3
	vpsrld %xmm1, %ymm2, %ymm2
	vpor %ymm2, %ymm3, %ymm2
	movups (%rdi), %ymm3
	vpxor %ymm3, %ymm2, %ymm2
	lea 32(%rdi), %rdi
	dec %rdx
	jmp 1b
2:	and 0x1F, %rsi # length mod 32 -> %rsi
	mov %rsi, %rdx
	shr $0x4, %rdx # (length mon 32) / 16 -> %rdx
	je 3f
	# Process 16bytes
	lea 16(%rdi), %rdi
	sub $0x10, %rsi
3:	mov %rsi, %rdx
	shr $0x3, %rdx
	je 4f
	# Process 8 bytes
	lea 8(%rdi), %rdi
	sub $0x8, %rsi
4:	mov %rsi, %rdx
	shr $0x2, %rdx
	je 5f
	# Process 4 bytes
	lea 4(%rdi), %rdi
	sub 0x4, %rsi
5:	mov %rsi, %rdi
	shr $0x1, %rdi
	je 6f
	# Process 2 bytes
	lea 2(%rdi), %rdi
	sub 0x2, %rsi
6:	and 0x1, %rsi
	je 7f
	# Process 1 byte
7:	ret
	

