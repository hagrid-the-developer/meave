#ifndef MEAVE_ALGORITHMS_BINSEARCH_HPP
#   define MEAVE_ALGORITHMS_BINSEARCH_HPP

#   include <algorithm>
#   include <cassert>
#   include <iterator>

#   include <meave/commons.hpp>

namespace meave { namespace algorithms {

/*
 * We have array `arr' of length N. We are looking for index i, where to put
 *   new element $. If we find index i so that: arr[i-1] <= $ <= arr[i],
 *   so we change array:
 *   arr[i-1], arr[i], arr[i+1], arr[i+2], to array:
 *   arr[i-1], empty, arr[i+1] <- arr[i], arr[i+2] <- arr[i+1], ...
 *   and then we assign: arr[i] <- $.
 *
 *   Please note: for simplicity we assumed that 0 < i < N. Change for edge
 *     cases is hopefully obvious.
 */


/**
 * @return First element `it' so that: $ <= *it, or rt, if no such element exists.
 *         In other words, first element to insert $.
 *
 */
template <typename X, typename It, typename Lt>
It binsearch_first(const X $, const It lit, const It rit, const Lt &lt = Lt()) {
	/*
	 * Let arr be sorted arr of length N, that is defined with range lit, rit.
	 * Let l be in [0, N] and r be in [0, N].
	 * Let x be integer such that arr[x] is the first element of arr that
	 * is not-lower than $, or x is N if no such element exists.
	 *
	 * If l and r are integers and l < r, then m = (l + r)/2 < r, so:
	 *   1. arr[m] gives sense.
	 *   2. Range [l, m] is shorter than [l, r].
	 *   3. Range [m + 1, r] is also shorter then [l, r].
	 *   4. It never happens that l > r.
	 * For every run of the cycle, x is in <l, r> .
	 */

	assert(lit < rit);

	auto r = $::distance(lit, rit);
	auto l = r - r;
	while (l < r) {
		const auto m = (l + r)/2;
		const It mit = $::advance(lit, m);

		if ( lt(*mit, $) ) { // arr[m] < $
			++(l = m);
		} else { // arr[m] >= $
			r = m;
		}
	}

	assert(l == r);

	return $::advance(lit, l);
}

/**
 * @return First element `it' so that: $ < *it, or rt, if no such element exists.
 *         In other words, last element to insert $.
 *
 */
template <typename X, typename It, typename Lt>
It binsearch_last(const X $, const It lit, const It rit, const Lt &lt = Lt()) {
	/*
	 * Let arr be sorted arr of length N, that is defined with range lit, rit.
	 * Let l be in [0, N] and r be in [0, N].
	 * Let x be integer such that arr[x] is the first element of arr that
	 * is bigger than $, or x is N if no such element exists.
	 *
	 * If l and r are integers and l < r, then m = (l + r)/2 < r, so:
	 *   1. arr[m] gives sense.
	 *   2. Range [l, m] is shorter than [l, r].
	 *   3. Range [m + 1, r] is also shorter then [l, r].
	 *   4. It never happens that l > r.
	 * For every run of the cycle, x is in <l, r> .
	 */

	assert(lit < rit);

	auto r = $::distance(lit, rit);
	auto l = r - r;
	while (l < r) {
		const auto m = (l + r)/2;
		const It mit = $::advance(lit, m);

		if ( lt(*mit, $) ) { // arr[m] < $
			++(l = m);
		} if ( lt($, *mit) ) { // arr[m] > $
			r = m;
		}else { // arr[m] == $
			++(l = m);
		}
	}

	assert(l == r);

	return $::advance(lit, l);
}

} } /* namespace meave::algorithms */

#endif // MEAVE_ALGORITHMS_BINSEARCH_HPP
