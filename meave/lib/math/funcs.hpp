#ifndef MEAVE_LIB_MATH_FUNCS_HPP
#	define MEAVE_LIB_MATH_FUNCS_HPP

#	include <cmath>

namespace meave { namespace math {

template <typename Float>
Float sigmoid(const Float x)
{
	const Float exp_value = exp(-x);
	return 1 / (1 + exp_value);
}

template<typename T>
T abs(const T &x) {
	return x >= T() ? x : -x;
}

template<typename T>
T abs_err(const T& x, const T &y) {
	return x >= y ? x - y : y - x;
}

template<typename T>
T rel_err(const T &x, const T &y) {
	const T abs_x = x >= T() ? x : -x;
	return abs_err(x, y) / abs_x;
}

} } /* namespace meave::math */

/* Simd Part */
#	include "meave/lib/simd.hpp"

namespace meave { namespace math {

meave::simd::AVX abs(const meave::simd::AVX &x) noexcept {
	// http://stackoverflow.com/questions/5508628/how-to-absolute-2-double-or-4-floats-using-sse-instruction-set-up-to-sse4
	return ::meave::simd::AVX{{ _mm256_andnot_ps(_mm256_set1_ps(-0.f), x) }};
}

} } /* namespace meave::math */

#endif
