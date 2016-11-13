#include <boost/crc.hpp>

#include <algorithm>
#include <memory>
#include <iostream>

#include "lib/utils.hpp"

#include "crc/crc_test.hpp"
#include "crc/funcs.hpp"

/**
 * Compares speed and return value of various functions for calculation of CRC.
 */

namespace {

	typedef ::uint32_t (*CRCCalcForCaller)(const ::uint8_t*, const ::size_t, const ::uint32_t);

	struct CRC {
		static const ::size_t BITS = 32;
		static const ::uint32_t POLY = 0x1EDC6F41;
		static const ::uint32_t INIT_REM = 0;
		static const ::uint32_t FINAL_XOR = 0;
		static const bool REFLECT_INPUT = true;
		static const bool REFLECT_REMAINDER = false;

		typedef boost::crc_optimal<BITS, POLY, INIT_REM, FINAL_XOR, REFLECT_INPUT, REFLECT_REMAINDER> Calc;
	};

	::uint32_t calc_with_boost(const ::uint8_t *arr, const ::size_t len) {
		// http://www.boost.org/doc/libs/1_55_0/libs/crc/crc.html
		CRC::Calc calc;
		calc.process_bytes(&arr[0], len);
		return calc.checksum();
	}

	template <CRCCalcForCaller F>
	::uint32_t calc_caller(const ::uint8_t *arr, const ::size_t len) noexcept {
		return F(arr, len, CRC::INIT_REM);
	}

} /* anonymous namespace */

int main(void) {
	namespace mct = meave::crc::test;
	/* */
	mct::init_comparisions();
	/* */
	mct::compare_output("crc32_intel_asm", calc_with_boost, calc_caller<crc32_intel_asm>);
	$::cerr << $::endl;
	$::cerr << $::endl;
	/* */
	mct::measure_speed("calc_with_boost", calc_with_boost);
	$::cerr << $::endl;
	mct::measure_speed("sse42_crc32", sse42_crc32);
	$::cerr << $::endl;
	mct::measure_speed("crc32_intel_asm", calc_caller<crc32_intel_asm>);
	$::cerr << $::endl;
	mct::measure_speed("crc32_intel", calc_caller<crc32_intel>);
	/* */
	return 0;
}
