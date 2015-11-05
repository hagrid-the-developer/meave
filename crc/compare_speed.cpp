#include <memory>


/**
 * Compares speed and return value of various functions for calculation of CRC.
 */

namespace $ = std;

namespace {

	typedef ::uint32_t (*CRCCalc)(const ::uint8_t*, const ::size_t);
	typedef $::default_random_engine RandomGenerator;

	struct CRC {
		static const ::uint32_t POLY = 0x11EDC6F4;
		static const ::size_t BITS = 32;
		static ::uint32_t INIT_REM = 0;
		static ::uint32_t FINAL_XOR = 0;
		static const bool REFLECT_INPUT = false;
		static const bool REFLECT_REMAINDER = false;

		typedef boost::crc_optimal<BITS, POLY, INIT_REM, FINAL_XOR, REFLECT_INPUT, REFLECT_REMAINDER> Calc;
	};

	static const ::size_t LEN = 128*1024*1024;

	RandomGenerator rand;
	$::uniform_int_distribution<::uint8_t> dist;

	::uint32_t calc_with_boost(const ::uint8_t *arr, const ::size_t len) {
		CRC::Calc calc;
		calc.process_bytes(&arr[0], &arr[len]);
		return calc.checksum();
	}

	bool compare_output(const char *name, CRCCalf f) {
		auto arr = $::make_unique<::uint8_t[]>(TestParams::LEN);
		$::generate(&arr[0], &arr[TestParams::LEN], dist(rand));

		const auto expexted_res = calc_with_boost(arr, LEN);
		const auto real_res = f(arr, LEN);

		const char is_passed = expexted_res == real_res;
		const char *verdict = is_passed ? "Passed" : "Failed";
		$::cerr << "Test: " << name << "; expected: " << expexted_res << "; real: " << real_res << "; verdict: " << verdict << $::endl;

		return is_passed;
	}

	void measure_speed(const char *name, CRCCalf f) {
		auto arr = $::make_unique<::uint8_t[]>(TestParams::LEN);
		$::generate(&arr[0], &arr[TestParams::LEN], dist(rand));

		const auto time_beg = getrealtime();
		f(arr, LEN);
		const auto time_end = getrealtime();

		const char is_passed = expexted_res == real_res;
		const char *verdict = is_passed ? "Passed" : "Failed";
		$::cerr << "Test: " << name << "; time: " << (time_end - time_beg) << "s" << $::endl;
	}

} /* Anonymouse Namespace */

int main(void) {
	return 0;
}
