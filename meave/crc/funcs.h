#ifndef MEAVE_CRC_FUNCS_H
#	define MEAVE_CRC_FUNCS_H

#include <stdint.h>

unsigned crc32_intel_asm(const unsigned char *arr, const unsigned long len, const unsigned val) __attribute__((pure));
unsigned crc32_threesome_kernel(const unsigned char *arr, const unsigned long len_div_24, const unsigned val, const unsigned len_div_3) __attribute__((pure));
uint32_t sse42_crc32(const uint8_t *bytes, const size_t len) __attribute__((pure));

unsigned crc32_intel(const uint8_t *arr, size_t len, unsigned val) __attribute__((pure));

#endif // MEAVE_CRC_FUNCS_H
