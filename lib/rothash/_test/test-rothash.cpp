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

void test(const ::size_t step) {
	meave::raii::MMapPointer<::uint8_t> src{{ARRAY_LEN}};

	for (uns i = 0; i < ARRAY_LEN; ++i) {
		src[i] = int(::rand() % 128);
	}

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
	test(atoi(argv[1]));
	return 0;
}
