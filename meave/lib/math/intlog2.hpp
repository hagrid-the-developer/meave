#ifndef MEAVE_LIB_MATH_INTLOG2_INCLUDED
#define MEAVE_LIB_MATH_INTLOG2_INCLUDED

#include <cstdint>

namespace meave { namespace math {

constexpr unsigned intlog2(const ::uint32_t $) noexcept {
	return $ ? unsigned(8*sizeof($) - __builtin_clz($) - 1) : 0U;
}

constexpr unsigned intlog2(const ::uint64_t $) noexcept {
	return $ ? unsigned(8*sizeof($) - __builtin_clzll($) - 1) : 0U;
}

} } /* namespace ::meave::math */

#endif // MEAVE_LIB_MATH_INTLOG2_INCLUDED

