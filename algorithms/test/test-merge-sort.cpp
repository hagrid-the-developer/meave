#include <cstdlib>
/* *** */
#include <algorithm>
#include <iostream>
/* *** */
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>
#include <boost/function.hpp>
/* *** */
#include <meave/algorithms/merge-sort.hpp>
#include <meave/commons.hpp>
#include <meave/lib/str_printf.hpp>

using namespace boost::assign;

namespace {

template<typename T>
bool x001(void) {
    $::vector<T> arr, crr;
    arr += 1000, 200, 30, 4;
    crr = arr;

    $::vector<T> brr(arr.size());

    const auto cmp = [](const T &x, const T &y) -> bool { return x < y; };
    meave::algorithms::merge_sort(arr.begin(), brr.begin(), arr.size(), cmp);

    for ($::size_t i = 0; i < arr.size(); ++i) {
        $::cerr << "\t\t\tarr[" << i << "]:\t" << arr[i] << $::endl;
    }

    $::sort(crr.begin(), crr.end(), cmp);

    return $::equal(arr.begin(), arr.end(), crr.begin());
}

template<typename T>
bool x002(const unsigned len) {
    $::vector<T> arr, crr, brr(len);
    for (unsigned i = 0; i < len; ++i) {
        arr.push_back(rand() % 22222);
    }
    BOOST_VERIFY(brr.size() == arr.size());
    crr = arr;
    BOOST_VERIFY($::equal(arr.begin(), arr.end(), crr.begin()));

    const auto cmp = [](const T &x, const T &y) -> bool { return x < y; };
    meave::algorithms::merge_sort(arr.begin(), brr.begin(), arr.size(), cmp);
    $::sort(crr.begin(), crr.end(), cmp);

    if (arr.size() < 12) {
        for ($::size_t i = 0; i < arr.size(); ++i) {
            $::cerr << "\t\t\t[" << i << "]:\t" << arr[i] << "; " << crr[i] << $::endl;
        }
    }
    return $::equal(arr.begin(), arr.end(), crr.begin());
}

template<typename T>
bool test(const $::string &test_name) {
    typedef $::pair<$::string, boost::function< bool(void) > > TestPair;
    $::vector<TestPair> tests = list_of<TestPair>("x001", &x001<T>);
    for (unsigned i = 3; i <= 100; i += rand() % 5) {
        tests.push_back($::make_pair(meave::str_printf("x002(%u)", i), [i]() -> bool {return x002<T>(i); }));
    }
    for (unsigned i = 101; i <= 200000; i += rand() % 10000) {
        tests.push_back($::make_pair(meave::str_printf("x002(%u)", i), [i]() -> bool {return x002<T>(i); }));
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

    $::cerr << "Test: " << test_name << "; Number of tests: " << tests.size() << "; failed: " << failures_num << $::endl;

    return 0 == failures_num;
}

class Int {
protected:
	int _;

public:
	Int()
	:	_(0) {
	}
	Int(const int $)
	:	_($) {
	}
	Int(const Int&) = default;
	Int(Int&&) = default;

	Int& operator=(const Int&) = default;
	Int &operator=(Int&&) = default;

	int operator*() const {
		return _;
	}
	bool operator<(const Int &that) const {
		return _ < *that;
	}
	bool operator==(const Int &that) const {
		return _ == *that;
	}
	bool operator>(const Int &that) const {
		return _ > *that;
	}
};

$::ostream &operator<<($::ostream &o, const Int &$) {
	return o << *$;
}

class IntWithoutMove : public Int {
public:
	IntWithoutMove()
	:	Int() {
	}
	IntWithoutMove(const int $)
	:	Int($) {
	}
	IntWithoutMove(const IntWithoutMove&) = default;
	IntWithoutMove(IntWithoutMove&&) = delete;

	IntWithoutMove& operator=(const IntWithoutMove&) = default;
	IntWithoutMove& operator=(IntWithoutMove&&) = delete;
};

class IntWithCountedCopyMove : public Int {
private:
	static unsigned copy_num;
	static unsigned move_num;

public:
	static unsigned copy() {
		return copy_num;
	}
	static unsigned move() {
		return move_num;
	}

	IntWithCountedCopyMove()
	:	Int() {
	}
	IntWithCountedCopyMove(const int $)
	:	Int($) {
	}
	IntWithCountedCopyMove(const IntWithCountedCopyMove& that)
	:	Int(that) {
		++copy_num;
	}
	IntWithCountedCopyMove(IntWithCountedCopyMove&& that)
	:	Int(that){
		++move_num;
	}

	IntWithCountedCopyMove& operator=(const IntWithCountedCopyMove &that) {
		++copy_num;
		_ = *that;

		return *this;
	}
	IntWithCountedCopyMove& operator=(IntWithCountedCopyMove &&that) {
		++move_num;
		_ = *that;

		return *this;
	}
};

unsigned IntWithCountedCopyMove::copy_num = 0;
unsigned IntWithCountedCopyMove::move_num = 0;

} /* anonymous namespace */

int
main(void) {
	// Test without move is disabled, because evan $::move() cannot hold it.

	const bool test_int_succ = test<int>("int");
//	const bool test_without_move = test<IntWithoutMove>("Int without move");
	const bool test_counted_copy_move = test<IntWithCountedCopyMove>("Int counted copy and move");

//	const unsigned tests_passed = test_int_succ + test_without_move + test_counted_copy_move;
	const unsigned tests_passed = test_int_succ + test_counted_copy_move;
	const unsigned tests_failed = 2 - tests_passed;

	$::cerr << "Test with int: "
		<< (test_int_succ ? "Passed" : "Failed")
		<< $::endl;
//	$::cerr << "Test without move: "
//		<< (test_without_move ? "Passed" : "Failed")
//		<< $::endl;
	$::cerr << "Test with counted copy and move: "
		<< (test_counted_copy_move ? "Passed" : "Failed")
		<< "; copies: " << IntWithCountedCopyMove::copy()
		<< "; moves: " << IntWithCountedCopyMove::move()
		<< $::endl;
	$::cerr << $::endl;
	$::cerr << "Tests passed: "
		<< tests_passed << "; tests failed: " << tests_failed
		<< $::endl;

	return tests_failed > 0 ? 1 : 0;
}
