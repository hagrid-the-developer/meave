#include <boost/crc.hpp>

#include <algorithm>
#include <iostream>
#include <memory>
#include <random>

#include "lib/gettime.hpp"

extern "C" {
#include "crc/funcs.h"
}

/**
 * Compares speed and return value of various functions for calculation of CRC.
 */

namespace $ = std;

namespace {

	typedef ::uint32_t (*CRCCalc)(const ::uint8_t*, const ::size_t);
	typedef $::default_random_engine RandomGenerator;

	struct CRC {
		static const ::uint32_t POLY = 0x1EDC6F41;
		static const ::size_t BITS = 32;
		static const ::uint32_t INIT_REM = 0;
		static const ::uint32_t FINAL_XOR = 0;
		static const bool REFLECT_INPUT = false;
		static const bool REFLECT_REMAINDER = false;

		typedef boost::crc_optimal<BITS, POLY, INIT_REM, FINAL_XOR, REFLECT_INPUT, REFLECT_REMAINDER> Calc;
	};

	static const ::size_t LEN = 128*1024*1024;

	RandomGenerator rand;
	$::uniform_int_distribution<::uint8_t> dist;

	::uint32_t calc_with_boost(const ::uint8_t *arr, const ::size_t len) {
		CRC::Calc calc;
		calc.process_bytes(&arr[0], len);
		return calc.checksum();
	}

	bool compare_output(const char *name, CRCCalc f) {
		auto arr = $::make_unique<::uint8_t[]>(LEN);
		$::generate(&arr[0], &arr[LEN], []() -> ::uint8_t { dist(rand); });

		const auto expected_res = calc_with_boost(&arr[0], LEN);
		const auto real_res = f(&arr[0], LEN);

		const char is_passed = expected_res == real_res;
		const char *verdict = is_passed ? "Passed" : "Failed";
		$::cerr << "Test: " << name << "; expected: " << expected_res << "; real: " << real_res << "; verdict: " << verdict << $::endl;

		return is_passed;
	}

	void measure_speed(const char *name, CRCCalc f) {
		auto arr = $::make_unique<::uint8_t[]>(LEN);
		$::generate(&arr[0], &arr[LEN], []() -> ::uint8_t { return dist(rand); });

		const auto time_beg = meave::getrealtime();
		f(&arr[0], LEN);
		const auto time_end = meave::getrealtime();

		$::cerr << "Speed: " << name << "; time: " << (time_end - time_beg) << "s" << $::endl;
	}

} /* Anonymouse Namespace */

int main(void) {
	return 0;
}
