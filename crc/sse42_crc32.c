#include <stdint.h>
#include <immintrin.h>

#include "funcs.h"

#include <stdio.h>
uint32_t sse42_crc32(const uint8_t *bytes, const size_t len) {
	uint32_t hash = 0;
	size_t i = 0;
	for (i=0;i<len;i++) {
		hash = _mm_crc32_u8(hash, bytes[i]);
	}
	return hash;
}

unsigned crc32_intel(const uint8_t *arr, size_t len, unsigned val) {
	const size_t unalign_bytes = (~(uintptr_t)arr + 1) & 7;
	fprintf(stderr, "%p: %ju %ju\n", arr, ((uintmax_t)(uintptr_t)arr) & 7, (uintmax_t)unalign_bytes);
	if (__builtin_expect(unalign_bytes, 0)) {
		val = crc32_intel_asm(arr, unalign_bytes, val);
		len -= unalign_bytes;
		arr += unalign_bytes;
	}
	while (len > 24*128) {
		val = crc32_threesome_kernel(arr, 128, val, 8*128);
		//val = crc32_intel_asm(arr, 24*128, val);
		len -= 24*128;
		arr += 24*128;
	}
	if (len > 24) {
		const size_t step_len_div24 = len/24;
		const size_t step_len_div3 = step_len_div24 * 8;
		const size_t step_len = step_len_div3 * 3;

		val = crc32_threesome_kernel(arr, step_len_div24, val, step_len_div3);
		//val = crc32_intel_asm(arr, step_len, val);
		len -= step_len;
		arr += step_len;
	}
	val = crc32_intel_asm(arr, len, val);

	return val;
}

