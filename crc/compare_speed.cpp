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

	typedef ::uint32_t (*CRCCalc)(const ::uint8_t*, const ::size_t, const ::uint32_t);
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
	static const ::size_t MAX_SUFF_LEN = 16;

	RandomGenerator rand;
	$::uniform_int_distribution<::uint8_t> dist;

	::uint32_t calc_with_boost(const ::uint8_t *arr, const ::size_t len, const ::uint32_t=0) {
		CRC::Calc calc;
		calc.process_bytes(&arr[0], len);
		return calc.checksum();
	}

	bool compare_output(const char *name, CRCCalc f) {
		auto arr = $::make_unique<::uint8_t[]>(LEN + MAX_SUFF_LEN);
		$::generate(&arr[0], &arr[LEN + MAX_SUFF_LEN], []() -> ::uint8_t { dist(rand); });

		bool is_passed_all = false;
		for (::size_t suff = 0; suff < MAX_SUFF_LEN; ++suff) {
			const ::size_t len = LEN + suff;
			const auto expected_res = calc_with_boost(&arr[0], len);
			const auto real_res = f(&arr[0], len, CRC::INIT_REM);

			const char is_passed = expected_res == real_res;
			const char *verdict = is_passed ? "Passed" : "Failed";
			$::cerr << "Test: " << name << "; suff_len: " << suff << "; expected: " << expected_res << "; real: " << real_res << "; verdict: " << verdict << $::endl;

			is_passed_all = is_passed_all && is_passed;
		}

		return is_passed_all;
	}

	void measure_speed(const char *name, CRCCalc f) {
		auto arr = $::make_unique<::uint8_t[]>(LEN + MAX_SUFF_LEN);
		$::generate(&arr[0], &arr[LEN + MAX_SUFF_LEN], []() -> ::uint8_t { return dist(rand); });

		for (::size_t suff = 0; suff < MAX_SUFF_LEN; ++suff) {
			const ::size_t len = LEN + suff;
			const auto time_beg = meave::getrealtime();
			f(&arr[0], len, CRC::INIT_REM);
			const auto time_end = meave::getrealtime();

			$::cerr << "Speed: " << name << "; suff_len: " << suff << "; time: " << (time_end - time_beg) << "s" << $::endl;
		}
	}

} /* Anonymouse Namespace */

int main(void) {
	compare_output("crc32_intel_asm", crc32_intel_asm);
	$::cerr << $::endl;
	$::cerr << $::endl;
	/* */
	measure_speed("calc_with_boost", calc_with_boost);
	$::cerr << $::endl;
	measure_speed("crc32_intel_asm", crc32_intel_asm);
	/* */
	return 0;
}
