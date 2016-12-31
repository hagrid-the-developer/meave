#include <cmath>
#include <cstdlib>
#include <iostream>

#include <meave/lib/math/funcs_approx.hpp>
#include <meave/lib/gettime.hpp>
#include <meave/lib/utils.hpp>
#include <meave/lib/raii/mmap_pointer.hpp>
#include <meave/lib/rothash/rothash.hpp>

#define ARRAY_LEN (20000000 + 0)

namespace {

template<::size_t STEP>
void test_by_steps() {
	meave::raii::MMapPointer<::uint8_t> src{{ARRAY_LEN}};

	for (uns i = 0; i < ARRAY_LEN; ++i) {
		src[i] = int(::rand() % 128);
	}

	{
		unsigned hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + STEP <= ARRAY_LEN; i += STEP) {
			hash ^= meave::rothash::naive<13>(&src[i], STEP);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << STEP << "; naive: " << hash << "; " << (e - b) << "seconds" << $::endl;
	}
	{
		unsigned hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + STEP <= ARRAY_LEN; i += STEP) {
			hash ^= meave::rothash::avx2(&src[i], STEP, 13);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << STEP << "; avx2:  " << hash << "; " << (e - b) << "seconds" << $::endl;
	}
}

void test() {
	meave::raii::MMapPointer<::uint8_t> src{{ARRAY_LEN}};

	for (uns i = 0; i < ARRAY_LEN; ++i) {
		src[i] = int(::rand() % 128);
	}

	{
		const double b = meave::getrealtime();
		const auto hash_naive = meave::rothash::naive<13>(&src[0], ARRAY_LEN);
		const double e = meave::getrealtime();
		$::cerr << "naive: " << hash_naive << "; " << (e - b) << "seconds" << $::endl;
	}
	{
		const double b = meave::getrealtime();
		const auto hash_avx2 = meave::rothash::avx2(&src[0], ARRAY_LEN, 13);
		const double e = meave::getrealtime();
		$::cerr << "avx2:  " << hash_avx2 << "; " << (e - b) << "seconds" << $::endl;
	}
}

} /* anonymous namespace */

int
main(void) {
	test();

	test_by_steps<4>();
	test_by_steps<8>();
	test_by_steps<12>();
	test_by_steps<16>();
	test_by_steps<20>();
	test_by_steps<24>();
	test_by_steps<32>();
	test_by_steps<64>();
	test_by_steps<100>();
	test_by_steps<104>();
	test_by_steps<108>();
	test_by_steps<256>();
	test_by_steps<1024>();
	test_by_steps<10000>();
	test_by_steps<20000>();
	test_by_steps<30000>();
	test_by_steps<40000>();
	test_by_steps<50000>();
	test_by_steps<60000>();
	test_by_steps<ARRAY_LEN>();
	return 0;
}
