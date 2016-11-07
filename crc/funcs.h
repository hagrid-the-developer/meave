#ifndef MEAVE_CRC_FUNCS_H
#	define MEAVE_CRC_FUNCS_H

#include <stdint.h>

unsigned crc32_intel_asm(const unsigned char *arr, const unsigned long len, const unsigned val) __attribute__((pure));
unsigned crc32_threesome_kernel(const unsigned char *arr, const unsigned long len, const unsigned val) __attribute__((pure));
uint32_t sse42_crc32(const uint8_t *bytes, const size_t len);

#endif // MEAVE_CRC_FUNCS_H
