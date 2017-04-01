#ifndef MEAVE_LIB_ROTHASH_ROTHASH_H
#	define MEAVE_LIB_ROTHASH_ROTHASH_H

#include <stdint.h>

uint32_t rothash_kernel_avx2(const uint8_t* /*array*/, uint64_t /*array_len*/, uint64_t /*rol_bits*/) __attribute__((pure));

#endif // MEAVE_LIB_ROTHASH_ROTHASH_H
