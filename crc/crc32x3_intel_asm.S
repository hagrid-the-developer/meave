	.globl	crc32x3_intel_asm
	.type	crc32x3_intel_asm,@function
	.align	16
crc32x3_intel_asm:
	# rdi -- pointer to an array
	# rsi -- length of the array
	# rdx -- value of the checksum
	# According AMD64 ABI, registers RBP, RBX, and R12-R15 are callee-save registers
	mov %rsi, %r11
	shr $0x3, %r11
1:	je 2f
	crc32q (%rdi), %rdx
	crc32q (%rdi, %rsi, 0x1), %r12
	crc32q (%rdi, %rsi, 0x2), %r13
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
	crc32b 0x0(%rdi, %rsi, 1), %rdx
	crc32b 0x1(%rdi, %rsi, 1), %rdx
	crc32b 0x2(%rdi, %rsi, 1), %rdx
	crc32b 0x3(%rdi, %rsi, 1), %rdx
	crc32b 0x0(%rdi, %rsi, 2), %rdx
	crc32b 0x1(%rdi, %rsi, 2), %rdx
	crc32b 0x2(%rdi, %rsi, 2), %rdx
	crc32b 0x3(%rdi, %rsi, 2), %rdx
	lea 0x4(%rdi), %rdi
3:	mov %rsi, %r11 # if (%rsi & $0x2)
	and $0x2, %r11
	je 4f
	crc32b 0x0(%rdi), %rdx
	crc32b 0x1(%rdi), %rdx
	crc32b 0x0(%rdi, %rsi, 1), %rdx
	crc32b 0x1(%rdi, %rsi, 1), %rdx
	crc32b 0x0(%rdi, %rsi, 2), %rdx
	crc32b 0x1(%rdi, %rsi, 2), %rdx
	lea 0x2(%rdi), %rdi
4:	mov %rsi, %r11 # if (%rsi & $0x1)
	and $0x1, %r11
	je 5f
	crc32b (%rdi), %rdx
	crc32b (%rdi, %rsi, 1), %rdx
	crc32b (%rdi, %rsi, 2), %rdx
5:	mov %rdx, %rax # return value
	ret
