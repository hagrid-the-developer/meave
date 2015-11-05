	.globl	crc32_intel_asm
	.type	crc32_intel_asm,@function
	.align	16
crc32_intel_asm:
	#rdi -- pointer to an array
	#rsi -- length of the array
	#rdx -- value of the checksum
	mov %rsi, %r11
	shr 0x3, %r11
1:	je 2f
	crc32q (%rdi), %rdx
	lea 0x8(%rdi), %rdi
	dec %r11
2:	# %rdi &= 7
	# if !(%rdi & 4): 3f
	crc32l (%rdi), %rdx
	lea 0x4(%rdi), %rdi
3:	if !(%rdi & 2): 4f
	crc32b (%rdi), %rdx
	crc32b 0x1(%rdi), %rdx
	lea 0x2(%rdi), %rdi
4:	if !(%rdi & 1): 5f
	crc32b (%rdi), %rdx
5:	ret
