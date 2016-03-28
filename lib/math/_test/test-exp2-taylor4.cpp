#include <cmath>
#include <iostream>

#include <meave/lib/math/funcs_approx.hpp>
#include <meave/lib/gettime.hpp>
#include <meave/lib/utils.hpp>
#include <meave/lib/raii/mmap_pointer.hpp>

#define ARRAY_LEN (200*1024 + 24)

namespace {

void test() {
	meave::raii::MMapPointer<float> src{{ARRAY_LEN}};
	meave::raii::MMapPointer<float> dst_exp2f{{ARRAY_LEN}};
	meave::raii::MMapPointer<float> dst_exp2_taylor{{ARRAY_LEN}};

	for (uns i = 0; i < ARRAY_LEN; ++i) {
		src[i] = dst_exp2f[i] = dst_exp2_taylor[i] = (int(i) - ARRAY_LEN/2) / 10000.f;
	}

	{
		const double b1 = meave::getrealtime();
		for (uns i = 0; i < ARRAY_LEN; ++i) {
			dst_exp2f[i] = ::exp2f(dst_exp2f[i]);
		}
		const double e1 = meave::getrealtime();
		$::cerr << "libc: " << (e1 - b1) << $::endl;
	}

	{
		const double b2 = meave::getrealtime();
		::exp2_taylor(*dst_exp2_taylor, *src, ARRAY_LEN);
		const double e2 = meave::getrealtime();
		$::cerr << "taylor4: " << (e2 - b2) << $::endl;
	}

	float max_abs_err = 0.f;
	float max_rel_err = 0.f;
	$::cout << "Value" << '|' << "Exp" << '|' << "Pade22" << $::endl;
	for (uns i = 0; i < ARRAY_LEN; ++i) {
		$::cout << src[i] << '|' << dst_exp2f[i] << '|' << dst_exp2_taylor[i] << $::endl;
		const float abs_err = ::fabs(dst_exp2f[i] - dst_exp2_taylor[i]);
		const float rel_err = abs_err / dst_exp2f[i];
		max_abs_err = $::max(abs_err, max_abs_err);
		max_rel_err = $::max(rel_err, max_rel_err);

	}
	$::cerr << "Max abs-err:" << max_abs_err << "; rel-err:" << max_rel_err << $::endl;
}

} /* anonymous namespace */

int
main(void) {
	test();

	return 0;
}
