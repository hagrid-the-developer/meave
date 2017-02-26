#ifndef MEAVE_LIB_HASH_FNV_HPP
#	define MEAVE_LIB_HASH_FNV_HPP

/*
 * https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
 */

#include <cstdint>
#include <meave/lib/vec.hpp>

namespace meave { namespace fnv {

namespace aux {

constexpr ::uint64_t fnv_offset_basis() noexcept {
	return ::uint64_t(0xcbf29ce484222325ULL);
}

constexpr ::uint64_t fnv_prime() noexcept {
	return ::uint64_t(0x100000001b3ULL);
}

} /* namespace aux */

::uint64_t naive1(const uint8_t *$, ::size_t len) noexcept {
	::uint64_t hash = aux::fnv_offset_basis();
	for (const ::uint8_t *p = $; len--; ++p) {
		hash *= aux::fnv_prime();
		hash ^= *p;
	}
	return hash;
}

::uint64_t naive1a(const uint8_t *$, ::size_t len) noexcept {
	::uint64_t hash = aux::fnv_offset_basis();
	for (const ::uint8_t *p = $; len--; ++p) {
		hash ^= *p;
		hash *= aux::fnv_prime();
	}
	return hash;
}

meave::vec::AVX naive1_vec(const ::uint8_t *$, ::size_t len) noexcept {
	assert(0 == len % 4);
	meave::vec::AVX hash{ .qw_ = { aux::fnv_offset_basis(), aux::fnv_offset_basis(), aux::fnv_offset_basis(), aux::fnv_offset_basis() }};
	for (const ::uint8_t *p = $; len; len -= 4) {
		hash.qw_[0] *= aux::fnv_prime();
		hash.qw_[1] *= aux::fnv_prime();
		hash.qw_[2] *= aux::fnv_prime();
		hash.qw_[3] *= aux::fnv_prime();
		hash.qw_[0] ^= *p++;
		hash.qw_[1] ^= *p++;
		hash.qw_[2] ^= *p++;
		hash.qw_[3] ^= *p++;
	}
	return hash;
}

meave::vec::AVX naive1a_vec(const ::uint8_t *$, ::size_t len) noexcept {
	assert(0 == len % 4);
	meave::vec::AVX hash{ .qw_ = { aux::fnv_offset_basis(), aux::fnv_offset_basis(), aux::fnv_offset_basis(), aux::fnv_offset_basis() }};
	for (const ::uint8_t *p = $; len; len -= 4) {
		hash.qw_[0] ^= *p++;
		hash.qw_[1] ^= *p++;
		hash.qw_[2] ^= *p++;
		hash.qw_[3] ^= *p++;
		hash.qw_[0] *= aux::fnv_prime();
		hash.qw_[1] *= aux::fnv_prime();
		hash.qw_[2] *= aux::fnv_prime();
		hash.qw_[3] *= aux::fnv_prime();
	}
	return hash;
}

#ifdef INTEL_WILL_EVER_REALEASE_64BIT_UINT_VEC_OPS
meave::vec::AVX avx1(const ::uint8_t *$, ::size_t len) noexcept {
	assert(0 == len % 4);
	meave::vec::AVX hash{ .i8_ = _mm256_set1_epi64(aux::fnv_offset_basis()) };
	meave::vec::AVX prime{ .i8_ = _mm256_set1_epi64(aux::fnv_prime()) };
	for (const ::uint8_t *p = $; len; len-=4) {
		hash.i8_ = _mm256_mul_epi64(hash.i8_, prime);
		meave::vec::AVX x{ .i8_ = _mm256_set_epi64(p[0], p[1], p[2],p[3])};
		hash.i8_ = _mm256_xor_si256(hash.i8_, x.i8_);
	}
	return hash;
}

meave::vec::AVX avx1a(const ::uint8_t *$, ::size_t len) noexcept {
	assert(0 == len % 4);
	meave::vec::AVX hash{ .i8_ = _mm256_set1_epi64(aux::fnv_offset_basis()) };
	meave::vec::AVX prime{ .i8_ = _mm256_set1_epi64(aux::fnv_prime()) };
	for (const ::uint8_t *p = $; len; len-=4) {
		meave::vec::AVX x{ .i8_ = _mm256_set_epi64(p[0], p[1], p[2],p[3])};
		hash.i8_ = _mm256_xor_si256(hash.i8_, x.i8_);
		hash.i8_ = _mm256_mul_epi64(hash.i8_, prime);
	}
	return hash;
}
#endif

} } /* namespace ::meave::fnv */

#endif // MEAVE_LIB_HASH_FNV_HPP
