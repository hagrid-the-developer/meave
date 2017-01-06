#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>

#include <meave/lib/math/funcs_approx.hpp>
#include <meave/lib/gettime.hpp>
#include <meave/lib/utils.hpp>
#include <meave/lib/raii/mmap_pointer.hpp>
#include <meave/lib/rothash/rothash.hpp>

#define ARRAY_LEN (20000000 + 0)

namespace {

template<::size_t STEP_MIN, ::size_t STEP_MAX = STEP_MIN>
void test() {
	meave::raii::MMapPointer<::uint8_t> src{{ARRAY_LEN}};

	for (uns i = 0; i < ARRAY_LEN; ++i) {
		src[i] = int(::rand() % 128);
	}

	const ::size_t step = ((STEP_MIN + ::rand() % (STEP_MAX - STEP_MIN + 1))/4)*4;
	{
		unsigned hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rothash::naive<13>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << "; naive: " << hash << "; " << (e - b) << "seconds" << $::endl;
	}
	{
		unsigned hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rothash::asm_avx2(&src[i], step, 13);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << "; asm_avx2:  " << hash << "; " << (e - b) << "seconds" << $::endl;
	}
	{
		unsigned hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rothash::avx2<13>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << "; avx2:  " << hash << "; " << (e - b) << "seconds" << $::endl;
	}

	$::cerr << $::endl;
}

} /* anonymous namespace */

int
main(int argc, char *argv[]) {
	if (argc > 1)
		::srand(atoi(argv[1]));

	test<4>();
	test<8>();
	test<12>();
	test<16>();
	test<20>();
	test<24>();
	test<32>();
	test<64>();
	test<100>();
	test<104>();
	test<108>();
	test<256>();
	test<1024>();
	test<10000>();
	test<20000>();
	test<30000>();
	test<40000>();
	test<50000>();
	test<60000>();
	test<4, 2048>();
	test<ARRAY_LEN>();
	return 0;
}
