#include <cstdlib>
/* *** */
#include <iostream>
#include <algorithm>
/* *** */
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>
#include <boost/function.hpp>
/* *** */
#include <meave/algorithms/merge-sort.hpp>
#include <meave/commons.hpp>

using namespace boost::assign;

static bool x001(void) {
    $::vector<int> arr, crr;
    arr += 1000, 200, 30, 4;
    crr = arr;

    $::vector<int> brr(arr.size());

    const auto cmp = [](const int x, const int y) -> bool { return x < y; };
    meave::algorithms::merge_sort(arr.begin(), brr.begin(), arr.size(), cmp);

    for ($::size_t i = 0; i < arr.size(); ++i) {
        $::cerr << "\t\t\tarr[" << i << "]:\t" << arr[i] << $::endl;
    }

    $::sort(crr.begin(), crr.end(), cmp);

    return $::equal(arr.begin(), arr.end(), crr.begin());
}

static bool x002(const unsigned len) {
    $::vector<int> arr, crr, brr(len);
    for (unsigned i = 0; i < len; ++i) {
        arr.push_back(rand() % 22222);
    }
    BOOST_VERIFY(brr.size() == arr.size());
    crr = arr;
    BOOST_VERIFY($::equal(arr.begin(), arr.end(), crr.begin()));

    const auto cmp = [](const int x, const int y) -> bool { return x < y; };
    meave::algorithms::merge_sort(arr.begin(), brr.begin(), arr.size(), cmp);
    $::sort(crr.begin(), crr.end(), cmp);

    if (arr.size() < 12) {
        for ($::size_t i = 0; i < arr.size(); ++i) {
            $::cerr << "\t\t\t[" << i << "]:\t" << arr[i] << "; " << crr[i] << $::endl;
        }
    }
    return $::equal(arr.begin(), arr.end(), crr.begin());
}

int
main(void) {
    typedef $::pair<$::string, boost::function< bool(void) > > TestPair;
    $::vector<TestPair> tests = list_of<TestPair>("x001", &x001);
    for (unsigned i = 3; i <= 100; i += rand() % 5) {
        tests.push_back($::make_pair($::string() + "x002(" + boost::lexical_cast<$::string>(i) + ")", [i]() -> bool {return x002(i); }));
    }
    for (unsigned i = 101; i <= 200000; i += rand() % 10000) {
        tests.push_back($::make_pair($::string() + "x002(" + boost::lexical_cast<$::string>(i) + ")", [i]() -> bool {return x002(i); }));
    }
    unsigned failures_num = 0;
    for (auto it(tests.begin()), e(tests.end()); it != e; ++it) {
        $::cerr << "\tRunning test: " << it->first << $::endl;
        const bool result = (it->second)();
        if (!result) {
            $::cerr << "\t\t...failed" << $::endl;
            ++failures_num;
        } else {
            $::cerr << "\t\t...success" << $::endl;
        }
    }

    $::cerr << "Number of tests: " << tests.size() << "; failed: " << failures_num << $::endl;

    return failures_num == 0 ? 0 : 1;
}
