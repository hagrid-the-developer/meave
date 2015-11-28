#ifndef MEAVE_ALGORITHMS_MERGE_SORT_HPP
#   define MEAVE_ALGORITHMS_MERGE_SORT_HPP

#   include <algorithm>
#   include <cassert>

#   include <meave/commons.hpp>

namespace meave { namespace algorithms {

    /**
     *
     */
    template <typename It, typename Lt>
    class MergeSort : protected Lt {
    public:
        typedef $::iterator_traits<It> Traits;
	typedef typename V::difference_type Distance;
	typedef typename Distance D;

    private:
        It b_, q_;
        D len_;

    public:
        MergeSort(const It &b, const It &q, const D len, const Lt &lt)
        :   Lt(lt),
            b_(b),
            q_(q),
            len_(len)
        {
            assert($::numeric_limits<D>::max()/2 >= len_);
        }

        /**
         *
         */
        It merge_(It ot, It it, const D from, const D step) const {
            const D mid = from + step;
            const D end = mid + step;
            D i = from;
            D j = mid;
            for (; i < $::min(mid, len_) && j < $::min(end, len_); ++ot) {
                if ((*this)(it[j], it[i])) {
                    *ot = $::move(it[j++]);
                } else {
                    *ot = $::move(it[i++]);
                }
            }
            for (; i < $::min(mid, len_); ++ot) {
                *ot = $::move(it[i++]);
            }
            for (; j < $::min(end, len_); ++ot) {
                *ot = $::move(it[j++]);
            }
            return ot;
        }
 
        D step_(const D step) const {
            return 1 << step;
        }

        void operator()(void) {
            It b = b_;
            It q = q_;
            D $ = 0;
            for (; step_($) < len_; ++$) {
                It o = q;
                for (D i = 0; i < len_; i += 2*step_($)) {
                    o = merge_(o, b, i, step_($));
                }
                $::swap(b, q);
            }
            if (1 == $ % 2) {
                BOOST_ASSERT(b == q_ && q == b_);
                $::move(q_, q_ + len_, b_);
            }
        }
    };

    template<typename It, typename Lt>
    void merge_sort(const It &b, const It &q, const D len, const Lt &lt) {
        MergeSort<It, Lt>(b, q, len, lt)();
    }
} }

#endif
