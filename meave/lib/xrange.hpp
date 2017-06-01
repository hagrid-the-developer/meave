#ifndef MEAVE_XRANGE_HPP_INCLUDED
#	define MEAVE_XRANGE_HPP_INCLUDED

#	include "num_it.hpp"

namespace meave {

namespace ___ {

template<typename Num>
class XRange {
private:
	Num b_, e_;

public:
	XRange(const Num b, const Num e)
	:	b_(b)
	,	e_(e) {
	}

	NumIt<Num> begin() const noexcept {
		return NumIt<Num>(b_);
	}

	NumIt<Num> end() const noexcept {
		return NumIt<Num>(e_);
	}
};

} /* namespace meave::___ */


template<typename Num>
___::XRange<Num> make_xrange(const Num b, const Num e) noexcept {
	return ___::XRange<Num>(b, e);
}

} /* namespace meave */

#endif // MEAVE_XRANGE_HPP_INCLUDED
