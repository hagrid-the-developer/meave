#ifndef MEAVE_LIB_ROTHASH_AVX2_HPP
#	define MEAVE_LIB_ROTHASH_AVX2_HPP

#include <cstdint>
#include <immintrin.h>

namespace meave { namespace vec {

union SSE {
	__m128i i4_;
	__m128 f4_;
	float sf_[8];
	::uint32_t dw_[8];
};

union AVX {
	__m256i i8_;
	__m256 f8_;
	float sf_[8];

	SSE sse_[2];
	::uint32_t dw_[8];
	::uint64_t qw_[4];
};

} } /* meave::vec */


namespace meave { namespace rothash {

namespace aux { namespace avx2 {

template<typename T = unsigned>
class HashBase {
protected:
	static const struct MaskBits {
		unsigned bits[sizeof(T)];

		MaskBits() {
			::uint8_t *p = reinterpret_cast<::uint8_t*>(bits);
			for (::size_t i = 0; i < sizeof(T); ++i) {
				for (::size_t j = 0; j < sizeof(T); ++j) {
					p[i*sizeof(T) + j] = j < i ? 0xFF : 0;
				}
			}
		}
	} mask;

	static unsigned unaligned_part(const ::uint8_t *x) noexcept {
		return static_cast<T>( reinterpret_cast< ::uintptr_t>(x) % sizeof(T) );
	}
};
template<typename T>
const typename HashBase<T>::MaskBits HashBase<T>::mask;

template <unsigned ROL_BITS>
class HashFunc : public aux::HashBase<unsigned> {
private:
	static meave::vec::AVX rol(const meave::vec::AVX x) noexcept {
		return meave::vec::AVX{ .i8_ = _mm256_or_si256( _mm256_slli_epi32(x.i8_, ROL_BITS), _mm256_srli_epi32(x.i8_, 32 - ROL_BITS) ) };
	}

	static ::uint32_t hash_aligned(const ::uint8_t *p, ::size_t len) noexcept {
		meave::vec::AVX hash;
		meave::vec::AVX u;

		hash.f8_ = _mm256_xor_ps(hash.f8_, hash.f8_);
		const ::size_t L = len;
		if (len >= 32) {
			do {
				hash = rol(hash);
				u.f8_ = _mm256_loadu_ps(reinterpret_cast<const float*>(&p[L - len]));
				hash.i8_ = _mm256_xor_si256(hash.i8_, u.i8_);
			} while ((len -= 32) >= 32);
			hash.sse_[0].i4_ = _mm_xor_si128(hash.sse_[0].i4_, hash.sse_[1].i4_);
		}
		if (len >= 16) {
			hash = rol(hash);
			u.sse_[0].f4_ = _mm_loadu_ps(reinterpret_cast<const float*>(&p[L - len]));
			hash.i8_ = _mm256_xor_si256(hash.i8_, u.i8_);
			len -= 16;
		}
		hash.qw_[0] ^= hash.qw_[1];
		if (len >= 8) {
			hash = rol(hash);
			u.qw_[0] = *reinterpret_cast<const ::uint64_t*>(&p[L - len]);
			hash.i8_ = _mm256_xor_si256(hash.i8_, u.i8_);
			len -= 8;
		}
		hash.dw_[0] ^= hash.dw_[1];
		if (len >= 4) {
			hash = rol(hash);
			u.dw_[0] = *reinterpret_cast<const ::uint32_t*>(&p[L - len]);
			hash.i8_ = _mm256_xor_si256(hash.i8_, u.i8_);
			len -= 4;
		}
		assert(!len);
/*
		::size_t l = len;
		for (; l >= sizeof(unsigned); l -= sizeof(unsigned)) {
			hash = rol(hash) ^ *u++;
		}
		hash = rol(hash) ^ (*u & mask.bits[l]);
*/
		return hash.qw_[0];
	}

	static unsigned hash_unaligned(const ::uint8_t *p, const ::size_t len) noexcept {
		unsigned hash = 0;
#if 0
		for (unsigned i = 0; ; ++i) {
			const ::size_t mod = i % sizeof(unsigned);
			if (0 == mod)
				hash = rol(hash);

			if (i >= len)
				break;

#			if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			const ::size_t shift = 8*mod;
#			elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
			const ::size_t shift = 8*(sizeof(unsigned) - mod - 1);
#			else
#			error Sorry, cannot determine endiadness
#			endif

			hash ^= p[i] << shift;
		}
#endif
		return hash;
	}

public:
	static unsigned hash(const ::uint8_t *p, const ::size_t len) noexcept {
		const auto alignment = unaligned_part(p);
		if (0 == alignment)
			return hash_aligned(p, len);

		return hash_unaligned(p, len);
	}
};

} } /* namespace aux::avx2 */

template <unsigned ROL_BITS>
uint32_t avx2(const ::uint8_t *p, const ::size_t len) noexcept {
	return aux::avx2::HashFunc<ROL_BITS>::hash(p, len);
}

} } /* meave::rothash */

#endif // MEAVE_LIB_ROTHASH_meave::vec::AVX2_HPP
