#ifndef MEAVE_NUM_IT_HPP_INCLUDED
#	define MEAVE_NUM_IT_HPP_INCLUDED

#	include <cstdint>
#	include <iterator>

#	include "utils.hpp"

namespace meave {

template <typename Num, typename Distance = ::intmax_t>
class NumIt {
public:
	typedef Num value_type;
	typedef const Num& reference;
	typedef const Num* pointer;
	typedef Distance difference_type;
	typedef $::random_access_iterator_tag iterator_category;

	NumIt() noexcept : _(Num()) { }
	explicit NumIt(const Num&$) noexcept : _($) { }
	NumIt(const NumIt&) = default;
	NumIt(NumIt&&) = default;

	reference base() const noexcept {
		return _;
	}
	reference operator*() const noexcept {
		return _;
	}
	NumIt& operator++() {
		++_;
		return *this;
	}
	NumIt& operator--() {
		--_;
		return *this;
	}

	/**
	 * This operator for conversion allows compiler to transform
	 *   all other operations with this iterator to operations
	 *   with Num.
	 */
	operator Num() const noexcept {
		return _;
	}
private:
	Num _;
};

template <typename Num>
NumIt<Num> num_it(const Num &$) {
	return NumIt<Num>($);
}

} /* namespace meave */

#endif // MEAVE_NUM_IT_HPP_INCLUDED
