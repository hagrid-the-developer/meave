#ifndef MEAVE_LIB_CHECKSUM_FLETCHER_INCLUDED
#define MEAVE_LIB_CHECKSUM_FLETCHER_INCLUDED

/*
 * https://software.intel.com/en-us/articles/fast-computation-of-fletcher-checksums
 * https://en.wikipedia.org/wiki/Fletcher%27s_checksum
 */

#include <cassert>
#include <cstdint>

namespace meave { namespace checksum {

typedef ::uint64_t FletcherChecksum __attribute__((vector_size(32)));

namespace aux {

union u4x64 {
	FletcherChecksum vec;
	::uint64_t x[4];
};

} /* namespace aux */

FletcherChecksum fletcher_aligned_slow(const char *p, const ::size_t size) {
	assert(0 == ::uintptr_t(p) % alignof(unsigned));

	aux::u4x64 $${};

	auto u = reinterpret_cast<const unsigned*>(p);
	for (::size_t i = 0; i < size / sizeof(*u); ++i) {
		$$.x[0] += u[i];
		$$.x[1] += $$.x[0];
		$$.x[2] += $$.x[1];
		$$.x[3] += $$.x[2];
	}

	return $$.vec;
}

} } /* meave::checksum */

#endif // MEAVE_LIB_CHECKSUM_FLETCHER_INCLUDED


















