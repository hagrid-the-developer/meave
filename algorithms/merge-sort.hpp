#ifndef MEAVE_ALGORITHMS_MERGE_SORT_HPP
#   define MEAVE_ALGORITHMS_MERGE_SORT_HPP

#   include <algorithm>
#   include <boost/assert.hpp>

#   include <meave/commons.hpp>

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
        $::size_t len_;

    public:
        MergeSort(const It &b, const It &q, const $::size_t len, const Lt &lt)
        :   Lt(lt),
            b_(b),
            q_(q),
            len_(len)
        {
            BOOST_VERIFY($::numeric_limits<$::size_t>::max()/2 >= len_);
        }

        /**
         *
         */
        It merge_(It ot, It it, const $::size_t from, const size_t step) const {
            const $::size_t mid = from + step;
            const $::size_t end = $::min(mid + step, len_);
            $::size_t i = from;
            $::size_t j = mid;
            for (; i < mid && j < end; ++ot) {
                if ((*this)(it[j], it[i])) {
                    *ot = it[j++];
                } else {
                    *ot = it[i++];
                }
            }
            for (; i < mid; ++ot) {
                *ot = it[i++];
            }
            for (; j < end; ++ot) {
                *ot = it[j++];
            }
            return ot;
        }
 
        $::size_t step_(const $::size_t step) const {
            return 1 << step;
        }

        void operator()(void) {
            It b = b_;
            It q = q_;
            $::size_t $ = 0;
            for (; step_($) < len_; ++$) {
                It o = q;
                for ($::size_t i = 0; i + step_($) < len_; i += 2*step_($)) {
                    o = merge_(o, b, i, step_($));
                }
                $::swap(b, q);
            }
            if (1 == $ % 2) {
                $::copy(b, q, q + len_);
            }
        }
    };

    template<typename It, typename Lt>
    void merge_sort(const It &b, const It &q, const $::size_t len, const Lt &lt) {
        MergeSort<It, Lt>(b, q, len, lt)();
    }
} }

#endif
