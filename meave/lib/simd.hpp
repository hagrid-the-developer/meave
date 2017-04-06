#ifndef MEAVE_LIB_SIMD_HPP
#	define MEAVE_LIB_SIMD_HPP

#include <cstdint>
#include <immintrin.h>

namespace meave { namespace simd {

union SSE {
	__m128i i4_;
	__m128 f4_;
	__m128d d2_;

	float sf_[4];
	float df_[2];

	::uint32_t dw_[4];
	::uint64_t qw_[2];
	::uint8_t b_[16];
};

union AVX {
	__m256i i8_;
	__m256 f8_;
	__m256d d4_;

	float sf_[8];
	float df_[4];

	SSE sse_[2];
	::uint32_t dw_[8];
	::uint64_t qw_[4];
	::uint8_t b_[32];
};

} } /* namespace meave::simd */

#endif // MEAVE_LIB_SIMD_HPP
