#ifndef MEAVE_LIB_MATH_FUNCS_HPP
#	define MEAVE_LIB_MATH_FUNCS_HPP

#	include <cmath>
#	include "meave/lib/simd.hpp"

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

#endif
