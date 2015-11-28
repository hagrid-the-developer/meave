#ifndef MEAVE_ALGORITHMS_WIKI_SORT_HPP
#   define MEAVE_ALGORITHMS_WIKI_SORT_HPP

#   include <algorithm>
#   include <cassert>
#   include <iterator>

#   include <meave/commons.hpp>

/**
 * WikiSort:
 * 	* https://en.wikipedia.org/wiki/Block_sort
 * 	* http://www.algoritmy.net/article/51222/Block-Merge-Sort .
 *
 */
namespace meave { namespace algorithms {

    /**
     *
     */
    template <typename It, typename Lt>
    class WikiSort : protected Lt {
    public:
	typedef $::iterator_traits<It> Traits;
	typedef typename V::difference_type Distance;
	typedef typename Distance D;

    private:
	It b_, q_;

	void rotate(const It b, const It e, const D amount) {
		const It _ = $::advance(b, amount);
		$::reverse(b, e);
		$::reverse(b, _);
		$::reverse(_, e);
	}

	D floor_pow_2(const D $) {
		static_assert(sizeof($) == 4 || sizeof($) == 8, "Wrong size of Distance type");

		D $$ = $;

#		define X(_) $$ |= $$ >> _
		X(1);
		X(2);
		X(4);
		X(8);
		X(16);

		if (sizeof($) > 4)
			X(32);
#		undef X

		return $$;
	}

    public:
        WikiSort(const It &b, const It &q, const Lt &lt)
        :   Lt(lt),
        ,   b_(b)
        ,   q_(q)
        { }

        void operator()(void) {
        }
    };

    template<typename It, typename Lt>
    void wiki_sort(const It &b, const It &q, const Lt &lt) {
        WikiSort<It, Lt>(b, q, lt)();
    }
} }

#endif
