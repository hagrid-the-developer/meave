#ifndef MEAVE_LIB_VEC_HPP
#	define MEAVE_LIB_VEC_HPP

#include <immintrin.h>

namespace meave { namespace vec {

union SSE {
	__m128i i4_;
	__m128 f4_;
	__m128d d2_;
	float sf_[4];
	::uint32_t dw_[4];
	::uint64_t qw_[2];
	::uint8_t b_[16];
};

union AVX {
	__m256i i8_;
	__m256 f8_;
	__m256d d4_;
	float sf_[8];

	SSE sse_[2];
	::uint32_t dw_[8];
	::uint64_t qw_[4];
	::uint8_t b_[32];
};

} } /* namespace ::meave::vec */

#endif // MEAVE_LIB_VEC_HPP
