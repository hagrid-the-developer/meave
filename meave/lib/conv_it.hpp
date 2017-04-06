#ifndef MEAVE_CONV_IT_HPP_INCLUDED
#	define MEAVE_CONV_IT_HPP_INCLUDED

#	include <cstdint>
#	include <iterator>

#	include "utils.hpp"

namespace meave {

template<typename BaseIt, typename Func>
class ConvIt : public BaseIt, private Func {
private:
	const BaseIt& base_it() const noexcept {
		return static_cast<const BaseIt&>(*this);
	}

public:
	typedef std::input_iterator_tag iterator_category;
	typedef decltype(**static_cast<ConvIt*>(nullptr)) value_type;
	typedef void pointer; // pointer has poor sense for this iterator
	typedef void reference; // reference has poor sense for this iterator

	ConvIt(const BaseIt &base_it, Func &&f)
	:	Func(std::move(f))
	,	BaseIt(base_it) {
	}

	ConvIt(const BaseIt &base_it, const Func &f)
	:	BaseIt(base_it)
	,	Func(f) {
	}

	auto operator*() const -> decltype( (*static_cast<const Func*>(nullptr))(*base_it()) ) {
		return (*this)(*base_it());
	}
};

template <typename BaseIt, typename Func>
ConvIt<BaseIt, Func> conv_it(const BaseIt &base_it, const Func &func) {
	return ConvIt<BaseIt, Func>(base_it, func);
}

#include <vector>

template <typename T, typename Func>
auto conv_it(T *p_t, const Func &func) -> ConvIt<typename std::vector<T>::iterator, Func> {
	typename std::vector<T>::iterator it(p_t);
	return ConvIt<decltype(it), Func>(it, func);
}

} /* namespace meave */

#endif // MEAVE_CONV_IT_HPP_INCLUDED
