#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>

#include <meave/lib/math/funcs_approx.hpp>
#include <meave/lib/gettime.hpp>
#include <meave/lib/utils.hpp>
#include <meave/lib/raii/mmap_pointer.hpp>
#include <meave/lib/rothash/rothash.hpp>

#define ARRAY_LEN (20000000)

namespace {

void test(const ::size_t step) {
	meave::raii::MMapPointer<::uint8_t> src{{ARRAY_LEN + 32}};

	for (uns i = 0; i < ARRAY_LEN; ++i) {
		src[i] = int(::rand() % 128);
	}

	if (0 == step % 4) {
		unsigned hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rothash::naive<13>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 16) {
		::uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rothash::sse_2<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2: " << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 16) {
		::uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rothash::sse_2_1<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2_1: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2_1: " << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 16) {
		::uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rothash::sse_2_2<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2_2: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2_2: " << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 16) {
		meave::vec::SSE hash{ .f4_ = _mm_setzero_ps() };
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash.i4_ = _mm_xor_si128( hash.i4_, meave::rothash::sse_2_3<13, 17>(&src[i], step).i4_ );
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2_3: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash.qw_[0] << ":" << hash.qw_[1] << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2_3: " << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 4) {
		uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rothash::naive2<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive2: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive2:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 4) {
		uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rothash::naive3<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive3: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive3:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 32) {
		unsigned hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rothash::asm_avx2(&src[i], step, 13);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " asm_avx2: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " asm_avx2:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 32) {
		unsigned hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rothash::avx2<13>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " avx2:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 32) {
		::uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rothash::avx2_2<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " avx2_2: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " avx2_2:" << $::setw(16) << "NA" << $::endl;
	}

	$::cerr << $::endl;
}

} /* anonymous namespace */

int
main(int argc, char *argv[]) {
	test(atoi(argv[1]));
	return 0;
}
