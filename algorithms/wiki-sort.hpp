#ifndef MEAVE_ALGORITHMS_WIKI_SORT_HPP
#   define MEAVE_ALGORITHMS_WIKI_SORT_HPP

#   include <algorithm>
#   include <boost/assert.hpp>

#   include <meave/commons.hpp>

/**
 * WikiSort: http://www.algoritmy.net/article/51222/Block-Merge-Sort .
 *
 */
namespace meave { namespace algorithms {

    /**
     *
     */
    template <typename It, typename Lt>
    class MergeSort : protected Lt {
    public:
        typedef $::iterator_traits<It> V;

    private:
        It b_, q_;

    public:
        MergeSort(const It &b, const It &q, const Lt &lt)
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
