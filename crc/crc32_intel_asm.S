	.globl	crc32_intel_asm
	.type	crc32_intel_asm,@function
	.align	16
crc32_intel_asm:
	# rdi -- pointer to an array
	# rsi -- length of the array
	# rdx -- value of the checksum
	mov %rsi, %r11
	shr 0x3, %r11
1:	je 2f
	crc32q (%rdi), %rdx
	lea 0x8(%rdi), %rdi
	dec %r11
2:	mov %rsi, %r11 # if (%rsi & 0x4)
	and 0x4, %r11
	je 3f
	crc32l (%rdi), %rdx
	lea 0x4(%rdi), %rdi
3:	mov %rsi, %r11 # if (%rsi & 0x2)
	and %r11, 0x2
	je 4f
	crc32b (%rdi), %rdx
	crc32b 0x1(%rdi), %rdx
	lea 0x2(%rdi), %rdi
4:	mov %rsi, %r11 # if (%rsi & 0x1)
	and r11, 0x1
	je 5f
	crc32b (%rdi), %rdx
5:	mov %rdx, %rax #r eturn value
	ret
