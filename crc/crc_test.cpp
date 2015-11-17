#include <algorithm>
#include <iostream>
#include <memory>
#include <random>

#include "lib/gettime.hpp"
#include "lib/utils.hpp"
#include "crc/crc_test.hpp"

namespace meave { namespace crc { namespace test {

namespace {

typedef $::default_random_engine RandomGenerator;

static const ::size_t LEN = 128*1024*1024;
static const ::size_t MAX_SUFF_LEN = 16;

RandomGenerator rand;
$::uniform_int_distribution<::uint8_t> dist;

} /* anonymous namespace */

void compare_output(const char *name, CRCFunc f_expected, CRCFunc f_real) {
	auto arr = $::make_unique<::uint8_t[]>(LEN + MAX_SUFF_LEN);
	$::generate(&arr[0], &arr[LEN + MAX_SUFF_LEN], []() -> ::uint8_t { dist(rand); });

	for (::size_t suff = 0; suff < MAX_SUFF_LEN; ++suff) {
		const ::size_t len = LEN + suff;
		const auto expected_result = f_expected(&arr[0], len);
		const auto real_result = f_real(&arr[0], len);

		const bool is_passed = expected_result == real_result;
		const char *verdict = is_passed ? "Passed" : "Failed";
		$::cerr << "Test: " << name
			<< "; suff_len: " << suff
			<< "; expected: " << expected_result
			<< "; real: " << real_result
			<< "; verdict: " << verdict
			<< ";" << $::endl;
	}
}

void measure_speed(const char *name, CRCFunc f) {
	auto arr = $::make_unique<::uint8_t[]>(LEN + MAX_SUFF_LEN);
	$::generate(&arr[0], &arr[LEN + MAX_SUFF_LEN], []() -> ::uint8_t { return dist(rand); });

	for (::size_t suff = 0; suff < MAX_SUFF_LEN; ++suff) {
		const ::size_t len = LEN + suff;
		const auto time_beg = meave::getrealtime();
		const auto res = f(&arr[0], len);
		const auto time_end = meave::getrealtime();

		$::cerr << "Speed: " << name
			<< "; suff_len: " << suff
			<< "; res: " << res
			<< "; time: " << (time_end - time_beg)
			<< "s;" << $::endl;
	}
}

} } } /* meave::crc::test */
