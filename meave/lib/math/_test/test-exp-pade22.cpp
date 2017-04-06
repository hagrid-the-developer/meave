#include <cmath>
#include <iostream>

#include <meave/lib/math/funcs_approx.hpp>
#include <meave/lib/gettime.hpp>
#include <meave/lib/utils.hpp>
#include <meave/lib/raii/mmap_pointer.hpp>

#define ARRAY_LEN (100*1024)

namespace {

void test() {
	meave::raii::MMapPointer<float> src{{ARRAY_LEN}};
	meave::raii::MMapPointer<float> dst_exp{{ARRAY_LEN}};
	meave::raii::MMapPointer<float> dst_pade22{{ARRAY_LEN}};

	for (uns i = 0; i < ARRAY_LEN; ++i) {
		src[i] = dst_exp[i] = dst_pade22[i] = i / 10000.f;
	}

	
	{
		const double b1 = meave::getrealtime();
		for (uns i = 0; i < ARRAY_LEN; ++i) {
			dst_exp[i] = ::exp(dst_exp[i]);
		}
		const double e1 = meave::getrealtime();
		$::cerr << "libc: " << (e1 - b1) << $::endl;
	}

	{
		const double b2 = meave::getrealtime();
		::exp_pade22(*dst_pade22, *src, ARRAY_LEN);
		const double e2 = meave::getrealtime();
		$::cerr << "pade22: " << (e2 - b2) << $::endl;
	}

	$::cout << "Value" << '|' << "Exp" << '|' << "Pade22" << $::endl;
	for (uns i = 0; i < ARRAY_LEN; ++i) {
		$::cout << src[i] << '|' << dst_exp[i] << '|' << dst_pade22[i] << $::endl;
	}
}

} /* anonymous namespace */

int
main(void) {
	test();

	return 0;
}
