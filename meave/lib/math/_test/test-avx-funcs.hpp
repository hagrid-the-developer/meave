#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>

#include <meave/commons.hpp>
#include <meave/lib/math.hpp>
#include <meave/lib/raii/mmap_pointer.hpp>
#include <meave/lib/gettime.hpp>

namespace {

constexpr ::size_t LEN = 10'000'000;
constexpr float min_diff = 1e-7;

void test() {
	meave::raii::MMapPointer<float> src{LEN};
	meave::raii::MMapPointer<float> dst_expected{LEN};
	meave::raii::MMapPointer<float> dst_real{LEN};

	for (::size_t i = 0; i < LEN; ++i) {
		src[i] = (rand()%2 ? +1.f : -1.f) * float(rand() % 100'000) / 10000.f;
	}

	const double expected_beg = meave::gettime();
	for (::size_t i = 0; i < LEN; ++i) {
		dst_expected[i] = meave::math::abs(src[i]);
	}
	const double expected_end = meave::gettime();

	const double real_beg = meave::gettime();
	for (::size_t i = 0; i < LEN; i += 8) {
		auto p_src = reinterpret_cast<meave::simd::AVX*>(&src[i]);
		auto p_dst = reinterpret_cast<meave::simd::AVX*>(&dst_expected[i]);
		*p_dst = meave::math::abs(*p_src);
	}
	const double real_end = meave::gettime();

	::size_t errors = 0;
	for (::size_t i = 0; i < LEN; ++i) {
		const float diff = dst_real[i] - dst_expected[i];
		const float abs_diff = diff > 0 ? diff : -diff;
		if (abs_diff > min_diff) {
			if (++errors < 15) {
				if (errors == 1)
					fprintf(stderr, "\n\t");
				else
					fprintf(stderr, "; ");
				fprintf(stderr, "i:%zu e:%f r:%f", i, dst_real[i], dst_expected[i]);
			}
		}
	}
	
	fprintf(stderr, "real-time: %lf; expected-time: %lf\n", (expected_end - expected_beg), (real_end - real_beg));
}

} /* anonymous namespace */

int
main(void) {
	test();

	return 0;
}
