#ifndef MEAVE_LIB_HASH_FNV_HPP
#	define MEAVE_LIB_HASH_FNV_HPP

/*
 * https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
 */

#include <cstdint>

namespace meave { namespace fnv {

namespace aux {

constexpr ::uint64_t fnv_offset_basis() noexcept {
	return ::uint64_t(0xcbf29ce484222325ULL);
}

constexpr ::uint64_t fnv_prime() noexcept {
	return ::uint64_t(0x100000001b3ULL);
}

} /* namespace aux */

::uint64_t fnv1(const uint8_t *$, ::size_t len) noexcept {
	::uint64_t hash = fnv_offset_basis();
	for (::uint8_t *p = $; len--; ++p) {
		hash *= fnv_prime();
		hash ^= *p;
	}
	return hash;
}

::uint64_t fnv1a(const uint8_t *$, ::size_t len) noexcept {
	::uint64_t hash = fnv_offset_basis();
	for (::uint8_t *p = $; len--; ++p) {
		hash ^= *p;
		hash *= fnv_prime();
	}
	return hash;
}

} } /* namespace ::meave::fnv */
