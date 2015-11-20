#include <stdint.h>
#include <immintrin.h>


uint32_t sse42_crc32(const uint8_t *bytes, const size_t len) {
	uint32_t hash = 0;
	size_t i = 0;
	for (i=0;i<len;i++) {
		hash = _mm_crc32_u8(hash, bytes[i]);
	}
	return hash;
}
