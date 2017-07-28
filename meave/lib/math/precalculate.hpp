#ifndef MEAVE_LIB_MATH_PRECALCULATE_INCLUDED
#define MEAVE_LIB_MATH_PRECALCULATE_INCLUDED

#ifndef __AVX__
#error AVX not enabled!
#endif

#include <cmath>
#include <meave/lib/simd.hpp>

namespace meave { namespace math {

namespace aux {

template <float F(float), unsigned SAMPLES = 1000>
class Precalculate {
private:
	enum {
		  S = SAMPLES
		, S_2 = S/2
		, S_21 = S/2 - 1
	};

	float data_[S];

	meave::simd::AVX min() const noexcept {
		return meave::simd::AVX{ .f8_ = _mm256_set1_ps(-5.f*float(S_21)/S_2) };
	}

	meave::simd::AVX max() const noexcept {
		return meave::simd::AVX{ .f8_ = _mm256_set1_ps(+5.f*float(S_21)/S_2) };
	}

public:
	Precalculate() noexcept {
		const ::ssize_t size = sizeof(data_)/sizeof(*data_);
		for (::ssize_t i = 0; i < S; ++i)
			data_[i] = F( 5.f*(float(i - S_2)/S_21) );
	}

	meave::simd::AVX operator()(meave::simd::AVX $) noexcept {
		$.f8_ = _mm256_min_ps(max().f8_, _mm256_max_ps(min().f8_, $.f8_));
		meave::simd::AVX index_f{ .f8_ = (S_2/5.f) * $.f8_ + _mm256_set1_ps(float(S_2)) };
		meave::simd::AVX index{ .i8_ = _mm256_cvtps_epi32(index_f.f8_) };
		return meave::simd::AVX{ .f8_ = _mm256_i32gather_ps(data_, index.i8_, 4) };
	}

	static Precalculate $;
};

template <float F(float), unsigned SAMPLES>
Precalculate<F, SAMPLES> Precalculate<F, SAMPLES>::$;

} /* namespace aux */

template <float F(float), unsigned SAMPLES = 1000>
meave::simd::AVX precalculated(const meave::simd::AVX $) noexcept {
	return aux::Precalculate<F, SAMPLES>::$($);
}

} } /* namespace ::meave::math */

#endif // MEAVE_LIB_MATH_PRECALCULATE_EXP_INCLUDED
