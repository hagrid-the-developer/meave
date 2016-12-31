#ifndef MEAVE_LIB_ROTHASH_ROTHASH_HPP
#	define MEAVE_LIB_ROTHASH_ROTHASH_HPP

extern "C" {
#include "rothash.h"
}

#include "rothash_naive.hpp"

namespace meave { namespace rothash {

	static inline ::uint32_t avx2(const ::uint8_t *p, const ::size_t len, const unsigned rot_bits) noexcept {
		return ::rothash_kernel_avx2(p, len, rot_bits);
	}

} } /* namespace meave::rothash */

#endif // MEAVE_LIB_ROTHASH_ROTHASH_HPP
