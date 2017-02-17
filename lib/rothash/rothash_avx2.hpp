#ifndef MEAVE_LIB_ROTHASH_AVX2_HPP
#	define MEAVE_LIB_ROTHASH_AVX2_HPP

#include <cstdint>
#include <immintrin.h>

namespace meave { namespace vec {

union SSE {
	__m128i i4_;
	__m128 f4_;
	__m128d d2_;
	float sf_[4];
	::uint32_t dw_[4];
	::uint64_t qw_[2];
	::uint8_t b_[16];
};

union AVX {
	__m256i i8_;
	__m256 f8_;
	__m256d d4_;
	float sf_[8];

	SSE sse_[2];
	::uint32_t dw_[8];
	::uint64_t qw_[4];
	::uint8_t b_[32];
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

		hash.f8_ = _mm256_xor_ps(hash.f8_, hash.f8_);
		const ::size_t L = len;
		if (__builtin_expect(len >= 32, 0)) {
			do {
				hash = rol(hash);
				meave::vec::AVX u {.i8_ = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(&p[L - len]))};
				hash.i8_ = _mm256_xor_si256(hash.i8_, u.i8_);
			} while (__builtin_expect((len -= 32) >= 32, 0));
			hash.sse_[0].i4_ = _mm_xor_si128(hash.sse_[0].i4_, hash.sse_[1].i4_);
		}
		if (__builtin_expect(len >= 16, 0)) {
			hash = rol(hash);
			meave::vec::AVX u {.sse_ = {{.i4_ = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(&p[L - len]))}, {.f4_ = _mm_setzero_ps()}}};
			hash.i8_ = _mm256_xor_si256(hash.i8_, u.i8_);
			len -= 16;
		}
		hash.qw_[0] ^= hash.qw_[1];
		if (__builtin_expect(len >= 8, 1)) {
			hash = rol(hash);
			meave::vec::AVX u{.i8_ = _mm256_broadcastq_epi64(*reinterpret_cast<const __m128i*>(&p[L - len]))};
			hash.i8_ = _mm256_xor_si256(hash.i8_, u.i8_);
			len -= 8;
		}
		hash.dw_[0] ^= hash.dw_[1];
		if (__builtin_expect(len >= 4, 1)) {
			hash = rol(hash);
			meave::vec::AVX u{.f8_ = _mm256_set1_ps(*reinterpret_cast<const float*>(&p[L - len]))};
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

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
class HashFunc2 : public aux::HashBase<unsigned> {
private:
	template <unsigned ROL_BITS>
	static meave::vec::AVX rol(const meave::vec::AVX x) noexcept {
		return meave::vec::AVX{ .i8_ = _mm256_or_si256( _mm256_slli_epi32(x.i8_, ROL_BITS), _mm256_srli_epi32(x.i8_, 32 - ROL_BITS) ) };
	}

	static ::uint64_t hash_aligned(const ::uint8_t *p, ::size_t len) noexcept {
		meave::vec::AVX hash0;
		meave::vec::AVX hash1;

		hash0.f8_ = _mm256_xor_ps(hash0.f8_, hash0.f8_);
		hash1.f8_ = _mm256_xor_ps(hash1.f8_, hash1.f8_);
		const ::size_t L = len;
		if (__builtin_expect(!len, 0))
			return 0;

		for (;; len -= 32) {
			hash0 = rol<ROL_BITS0>(hash0);
			hash1 = rol<ROL_BITS1>(hash1);
			hash0.i8_ = _mm256_xor_si256(hash0.i8_, *reinterpret_cast<const __m256i*>(&p[L - len]));
			hash1.i8_ = _mm256_xor_si256(hash1.i8_, *reinterpret_cast<const __m256i*>(&p[L - len]));
			if (__builtin_expect(len <= 32, 1))
				break;
		}
		hash0.sse_[0].i4_ = _mm_xor_si128(hash0.sse_[0].i4_, meave::vec::SSE{.f4_ = _mm256_extractf128_ps(hash0.f8_, 1)}.i4_);
		hash1.sse_[0].i4_ = _mm_xor_si128(hash1.sse_[0].i4_, meave::vec::SSE{.f4_ = _mm256_extractf128_ps(hash1.f8_, 1)}.i4_);
		meave::vec::SSE res0{ .d2_ = _mm_unpacklo_pd(hash0.sse_[0].d2_, hash1.sse_[0].d2_) };
		meave::vec::SSE res1{ .d2_ = _mm_unpackhi_pd(hash0.sse_[0].d2_, hash1.sse_[0].d2_) };
		res0.f4_ = _mm_xor_ps(res0.f4_, res1.f4_);
		res1.f4_ = _mm_movehdup_ps(res0.f4_);
		res0.f4_ = _mm_xor_ps(res0.f4_, res1.f4_);
		res1.f4_ = _mm_movehl_ps(res1.f4_, meave::vec::SSE{.i4_ = _mm_slli_epi64(res0.i4_, 32)}.f4_);
		res0.i4_ = _mm_or_si128(res0.i4_, res1.i4_);
		return _mm_cvtsi128_si64(res0.i4_);
	}

public:
	static ::uint64_t hash(const ::uint8_t *p, const ::size_t len) noexcept {
		return hash_aligned(p, len);
	}
};

} } /* namespace aux::avx2 */

template <unsigned ROL_BITS>
::uint32_t avx2(const ::uint8_t *p, const ::size_t len) noexcept {
	return aux::avx2::HashFunc<ROL_BITS>::hash(p, len);
}

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
::uint64_t avx2_2(const ::uint8_t *p, const ::size_t len) noexcept {
	return aux::avx2::HashFunc2<ROL_BITS0, ROL_BITS1>::hash(p, len);
}

namespace aux { namespace sse {

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
class HashFunc2 : public aux::HashBase<unsigned> {
private:
	template <unsigned ROL_BITS>
	static meave::vec::SSE rol(const meave::vec::SSE x) noexcept {
		return meave::vec::SSE{ .i4_ = _mm_or_si128( _mm_slli_epi32(x.i4_, ROL_BITS), _mm_srli_epi32(x.i4_, 32 - ROL_BITS) ) };
	}

	static ::uint64_t hash_aligned(const ::uint8_t *p, ::size_t len) noexcept {
		meave::vec::SSE hash0;
		meave::vec::SSE hash1;

		hash0.f4_ = _mm_xor_ps(hash0.f4_, hash0.f4_);
		hash1.f4_ = _mm_xor_ps(hash1.f4_, hash1.f4_);
		const ::size_t L = len;
		if (__builtin_expect(!len, 0))
			return 0;

		for (;; len -= 16) {
			hash0 = rol<ROL_BITS0>(hash0);
			hash1 = rol<ROL_BITS1>(hash1);
			hash0.i4_ = _mm_xor_si128(hash0.i4_, *reinterpret_cast<const __m128i*>(&p[L - len]));
			hash1.i4_ = _mm_xor_si128(hash1.i4_, *reinterpret_cast<const __m128i*>(&p[L - len]));
			if (__builtin_expect(len <= 16, 1))
				break;
		}
		meave::vec::SSE res0{ .d2_ = _mm_unpacklo_pd(hash0.d2_, hash1.d2_) };
		meave::vec::SSE res1{ .d2_ = _mm_unpackhi_pd(hash0.d2_, hash1.d2_) };
		res0.f4_ = _mm_xor_ps(res0.f4_, res1.f4_);
		res1.f4_ = _mm_movehdup_ps(res0.f4_);
		res0.f4_ = _mm_xor_ps(res0.f4_, res1.f4_);
		res1.f4_ = _mm_movehl_ps(res1.f4_, meave::vec::SSE{.i4_ = _mm_slli_epi64(res0.i4_, 32)}.f4_);
		res0.i4_ = _mm_or_si128(res0.i4_, res1.i4_);
		return _mm_cvtsi128_si64(res0.i4_);
	}

public:
	static ::uint64_t hash(const ::uint8_t *p, const ::size_t len) noexcept {
		return hash_aligned(p, len);
	}
};

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
class HashFunc2_1 : public aux::HashBase<unsigned> {
private:
	template <unsigned ROL_BITS>
	static meave::vec::SSE rol(const meave::vec::SSE x) noexcept {
		return meave::vec::SSE{ .i4_ = _mm_or_si128( _mm_slli_epi32(x.i4_, ROL_BITS), _mm_srli_epi32(x.i4_, 32 - ROL_BITS) ) };
	}

	static ::uint64_t hash_aligned(const ::uint8_t *p, ::size_t len) noexcept {
		meave::vec::SSE hash0;
		meave::vec::SSE hash1;

		hash0.f4_ = _mm_xor_ps(hash0.f4_, hash0.f4_);
		hash1.f4_ = _mm_xor_ps(hash1.f4_, hash1.f4_);
		const ::size_t L = len;
		if (__builtin_expect(!len, 0))
			return 0;

		for (;; len -= 16) {
			hash0 = rol<ROL_BITS0>(hash0);
			hash1 = rol<ROL_BITS1>(hash1);
			hash0.i4_ = _mm_xor_si128(hash0.i4_, *reinterpret_cast<const __m128i*>(&p[L - len]));
			hash1.i4_ = _mm_xor_si128(hash1.i4_, *reinterpret_cast<const __m128i*>(&p[L - len]));
			if (__builtin_expect(len <= 16, 1))
				break;
		}
		const __m128i in0 = hash0.i4_;
		const __m128i in1 = hash1.i4_;

		const __m128i xor64_0 = _mm_unpackhi_epi64(in0, in1);
		const __m128i xor64_1 = _mm_unpacklo_epi64(in0, in1);

		const __m128i xor64 = _mm_xor_si128(xor64_0, xor64_1);

		const __m128i xor32_0 = _mm_shuffle_epi32(xor64, _MM_SHUFFLE(3, 1, 2, 0));
		const __m128i xor32_1 = _mm_shuffle_epi32(xor64, _MM_SHUFFLE(2, 0, 3, 1));
		const __m128i xor32 = _mm_xor_si128(xor32_0, xor32_1);

		return _mm_cvtsi128_si64(xor32);
	}

public:
	static ::uint64_t hash(const ::uint8_t *p, const ::size_t len) noexcept {
		return hash_aligned(p, len);
	}
};

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
class HashFunc2_2 : public aux::HashBase<unsigned> {
private:
	template <unsigned ROL_BITS>
	static meave::vec::SSE rol(const meave::vec::SSE x) noexcept {
		return meave::vec::SSE{ .i4_ = _mm_or_si128( _mm_slli_epi32(x.i4_, ROL_BITS), _mm_srli_epi32(x.i4_, 32 - ROL_BITS) ) };
	}

	static ::uint64_t hash_aligned(const ::uint8_t *p, ::size_t len) noexcept {
		meave::vec::SSE hash0;
		meave::vec::SSE hash1;

		hash0.f4_ = _mm_xor_ps(hash0.f4_, hash0.f4_);
		hash1.f4_ = _mm_xor_ps(hash1.f4_, hash1.f4_);
		const ::size_t L = len;
		if (__builtin_expect(!len, 0))
			return 0;

		for (;; len -= 16) {
			hash0 = rol<ROL_BITS0>(hash0);
			hash1 = rol<ROL_BITS1>(hash1);
			hash0.i4_ = _mm_xor_si128(hash0.i4_, *reinterpret_cast<const __m128i*>(&p[L - len]));
			hash1.i4_ = _mm_xor_si128(hash1.i4_, *reinterpret_cast<const __m128i*>(&p[L - len]));
			if (__builtin_expect(len <= 16, 1))
				break;
		}

		const ::uint64_t x00 = _mm_extract_epi32(hash0.i4_, 0);
		const ::uint64_t x01 = _mm_extract_epi32(hash0.i4_, 1);
		const ::uint64_t x02 = _mm_extract_epi32(hash0.i4_, 2);
		const ::uint64_t x03 = _mm_extract_epi32(hash0.i4_, 3);
		const ::uint64_t x10 = _mm_extract_epi32(hash1.i4_, 0);
		const ::uint64_t x11 = _mm_extract_epi32(hash1.i4_, 1);
		const ::uint64_t x12 = _mm_extract_epi32(hash1.i4_, 2);
		const ::uint64_t x13 = _mm_extract_epi32(hash1.i4_, 3);

		const ::uint64_t y00 = x00 ^ x01;
		const ::uint64_t y01 = x02 ^ x03;
		const ::uint64_t y10 = x10 ^ x11;
		const ::uint64_t y11 = x12 ^ x13;

		const ::uint64_t a0 = y00 ^ y01;
		const ::uint64_t a1 = y10 ^ y11;

		return a1 << 32 | a0;
	}

public:
	static ::uint64_t hash(const ::uint8_t *p, const ::size_t len) noexcept {
		return hash_aligned(p, len);
	}
};

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
class HashFunc2_3 : public aux::HashBase<unsigned> {
private:
	template <unsigned ROL_BITS>
	static meave::vec::SSE rol(const meave::vec::SSE x) noexcept {
		return meave::vec::SSE{ .i4_ = _mm_or_si128( _mm_slli_epi32(x.i4_, ROL_BITS), _mm_srli_epi32(x.i4_, 32 - ROL_BITS) ) };
	}

	static __m128i get_mask(const ::size_t len) noexcept {
		static const __m128i masks[] = {
			  //            0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15
			  _mm_set_epi8(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00) // len ==  0
			, _mm_set_epi8(0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00) // len ==  1
			, _mm_set_epi8(0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00) // len ==  2
			, _mm_set_epi8(0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00) // len ==  3
			, _mm_set_epi8(0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00) // len ==  4
			, _mm_set_epi8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00) // len ==  5
			, _mm_set_epi8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00) // len ==  6
			, _mm_set_epi8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00) // len ==  7
			, _mm_set_epi8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00) // len ==  8
			, _mm_set_epi8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00) // len ==  9
			, _mm_set_epi8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00) // len == 10
			, _mm_set_epi8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00) // len == 11
			, _mm_set_epi8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00) // len == 12
			, _mm_set_epi8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00) // len == 13
			, _mm_set_epi8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00) // len == 14
			, _mm_set_epi8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00) // len == 15
			, _mm_set_epi8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF) // len == 16
		};

		return masks[len];
	}

	static meave::vec::SSE hash_aligned(const ::uint8_t *p, ::size_t len) noexcept {
		meave::vec::SSE hash0{ .f4_ = _mm_setzero_ps() };
		meave::vec::SSE hash1{ .f4_ = _mm_setzero_ps() };

		const ::size_t L = len;
		for (; __builtin_expect(len > 16, 0); len -= 16) {
			const auto x = *reinterpret_cast<const __m128i*>(&p[L - len]);
			hash0.i4_ = _mm_xor_si128(hash0.i4_, x);
			hash1.i4_ = _mm_xor_si128(hash1.i4_, x);
			hash0 = rol<ROL_BITS0>(hash0);
			hash1 = rol<ROL_BITS1>(hash1);
		}
		const __m128i mask = get_mask(len);
		const auto x = _mm_and_si128(mask, *reinterpret_cast<const __m128i*>(&p[L - len]));
		hash0.i4_ = _mm_xor_si128(hash0.i4_, x);
		hash1.i4_ = _mm_xor_si128(hash1.i4_, x);
		hash0 = rol<ROL_BITS0>(hash0);
		hash1 = rol<ROL_BITS1>(hash1);
		const __m128i in0 = hash0.i4_;
		const __m128i in1 = hash1.i4_;
		const __m128i xor64_0 = _mm_unpackhi_epi64(in0, in1);
		const __m128i xor64_1 = _mm_unpacklo_epi64(in0, in1);
		return meave::vec::SSE{ .i4_ = _mm_xor_si128(xor64_0, xor64_1) };
	}

public:
	static meave::vec::SSE hash(const ::uint8_t *p, const ::size_t len) noexcept {
		return hash_aligned(p, len);
	}
};

} } /* namespace aux::sse */

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
::uint64_t sse_2(const ::uint8_t *p, const ::size_t len) noexcept {
	return aux::sse::HashFunc2<ROL_BITS0, ROL_BITS1>::hash(p, len);
}

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
::uint64_t sse_2_1(const ::uint8_t *p, const ::size_t len) noexcept {
	return aux::sse::HashFunc2_1<ROL_BITS0, ROL_BITS1>::hash(p, len);
}

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
::uint64_t sse_2_2(const ::uint8_t *p, const ::size_t len) noexcept {
	return aux::sse::HashFunc2_2<ROL_BITS0, ROL_BITS1>::hash(p, len);
}

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
meave::vec::SSE sse_2_3(const ::uint8_t *p, const ::size_t len) noexcept {
	return aux::sse::HashFunc2_3<ROL_BITS0, ROL_BITS0>::hash(p, len);
}

} } /* meave::rothash */

#endif // MEAVE_LIB_ROTHASH_meave::vec::AVX2_HPP
