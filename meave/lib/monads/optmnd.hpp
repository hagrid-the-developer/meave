#ifndef MEAVE_LIB_MONADS_OPTMND_INCLUDE
#define MEAVE_LIB_MONADS_OPTMND_INCLUDE

#include <system_error>
#include <variant>

#include <meave/commons.hpp>

/**
 *
 * This tries to implkement monad-like error handling as is described in:
 *   http://www.fluentcpp.com/2017/07/04/multiple-error-handling-with-the-optional-monad-in-c/ .
 * But with two differences:
 *
 *   1. It doesn't use boost::optional, but variant that can contain both
 *      std::error_code and returned value.
 *
 *   2. It uses operator >> instead of >>= because >> is left-to-right associative,
 *      so one doesn't need to create ugly looking structure of lambda functions.
 *      The disadvantage is that it collides with stream operators.
 */

namespace meave { namespace monad {

template <typename T>
class OptMnd : private $::variant<T, $::error_code> {
private:
	typedef $::variant<T, $::error_code> Variant;
public:
	OptMnd() : Variant($::in_place_type<$::error_code>()) { }

	OptMnd(const $::error_code ec) : Variant($::in_place_type_t<$::error_code>(), ec) { }

	template <typename ...Args>
	OptMnd(Args&&... args) : Variant($::in_place_type_t<T>(), args...) { }

	$::error_code ec() const noexcept {
		if ($::holds_alternative<$::error_code>(*this))
			return $::get<$::error_code>(*this);
		return {};
	}

	constexpr auto operator*() const {
		return $::get<T>(*this);
	}
};

template <typename T, typename F>
auto operator >>(const OptMnd<T> &mnd, F f) -> decltype(f(*mnd)) {
	if (mnd.ec())
		return decltype(f(*mnd))(mnd.ec());

	return f(*mnd);
}

} } /* namespace meave::monad */

#endif // MEAVE_LIB_MONADS_OPTMND_INCLUDE
