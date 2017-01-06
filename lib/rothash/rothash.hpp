#ifndef MEAVE_LIB_ROTHASH_ROTHASH_HPP
#	define MEAVE_LIB_ROTHASH_ROTHASH_HPP

extern "C" {
#include "rothash.h"
}

#include "rothash_naive.hpp"
#include "rothash_avx2.hpp"

namespace meave { namespace rothash {

	static inline ::uint32_t asm_avx2(const ::uint8_t *p, const ::size_t len, const unsigned rot_bits) noexcept {
		return ::rothash_kernel_avx2(p, len, rot_bits);
	}

} } /* namespace meave::rothash */

#endif // MEAVE_LIB_ROTHASH_ROTHASH_HPP
