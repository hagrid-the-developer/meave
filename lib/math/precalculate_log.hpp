#ifndef MEAVE_LIB_MATH_PRECALCULATE_EXP_INCLUDED
#define MEAVE_LIB_MATH_PRECALCULATE_EXP_INCLUDED

#include <cmath>
#include <meave/lib/vec.hpp>

namespace meave { namespace math {

namespace aux {

class PrecalculateExp {
private:
	float data_[1000];

	meave::vec::AVX min() const noexcept {
		return meave::vec::AVX{ .f8_ = _mm256_set1_ps(-5.f*499.f/500.f) };
	}

	meave::vec::AVX max() const noexcept {
		return meave::vec::AVX{ .f8_ = _mm256_set1_ps(+5.f*499.f/500.f) };
	}

public:
	PrecalculateExp() noexcept {
		const ::ssize_t size = sizeof(data_)/sizeof(*data_);
		for (::ssize_t i = 0; i < size; ++i)
			data_[i] = ::expf( 5.f*(float(i - 500)/500.f) );
	}

	meave::vec::AVX operator()(meave::vec::AVX $) noexcept {
		$.f8_ = _mm256_min_ps(max().f8_, _mm256_max_ps(min().f8_, $.f8_));
		meave::vec::AVX index_f{ .f8_ = _mm256_set1_ps(500.f/5.f) * $.f8_ + _mm256_set1_ps(500.f) };
		meave::vec::AVX index{ .i8_ = _mm256_cvtps_epi32(index_f.f8_) };
		return meave::vec::AVX{ .f8_ = _mm256_i32gather_ps(data_, index.i8_, 4) };
	}
};

} /* namespace aux */

meave::vec::AVX precalculated_exp(const meave::vec::AVX $) noexcept {
	static aux::PrecalculateExp pexp;
	return pexp($);
}

} } /* namespace ::meave::math */

#endif // MEAVE_LIB_MATH_PRECALCULATE_EXP_INCLUDED
