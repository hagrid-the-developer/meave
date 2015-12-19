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
#include <meave/algorithms/binsearch.hpp>
#include <meave/commons.hpp>
#include <meave/lib/str_printf.hpp>

using namespace boost::assign;

namespace {

bool test(const $::string &test_name, const uns arr_len, const uns rand_range, const uns tests_num) {
	$::vector<uns> arr(arr_len);
	$::generate(arr.begin(), arr.end(), [rand_range]() -> uns { return ::rand() % rand_range; });
	$::sort(arr.begin(), arr.end());

	uns failures_num = 0;

	for (uns i = 0; i < tests_num; ++i) {
		const uns x = ::rand() % rand_range;
		const auto xit = meave::algorithms::binsearch_first(x, arr.begin(), arr.end());
		const bool xpassed = (xit == arr.end() || x <= *xit) &&
				     (xit == arr.begin() || x > xit[-1]);
		const auto yit = meave::algorithms::binsearch_last(x, arr.begin(), arr.end());
		const bool ypassed = (yit == arr.end() || x < *yit) &&
				     (yit == arr.begin() || x >= yit[-1]);
	
		failures_num += !xpassed + !ypassed;

		$::cerr << "\tTest: " << test_name << "; arr_len: " << arr_len << "; num: " << x << "; "
			<< (xpassed ? "Passed" : "Failed") << "; "
			<< (ypassed ? "Passed" : "Failed")
			<< $::endl;
	}

	return 0 == failures_num;
}

} /* anonymous namespace */

int
main(void) {
	uns tests_failed = 0;

	for (uns arr_len = 10000; arr_len < 130000; arr_len += 25000) {
		tests_failed += !test("11", arr_len, 11, 25);
		tests_failed += !test("121", arr_len, 121, 25);
		tests_failed += !test("1/100", arr_len, arr_len/100, 25);
		tests_failed += !test("1/1", arr_len, arr_len/1, 25);
	}
	$::cerr << "Number of failed tests: " << tests_failed << $::endl;

	return tests_failed > 0 ? 1 : 0;
}
