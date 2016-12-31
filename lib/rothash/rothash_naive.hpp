#ifndef MEAVE_LIB_ROTHASH_NAIVE_HPP
#	define MEAVE_LIB_ROTHASH_NAIVE_HPP

#include <cstddef>
#include <cstdint>

#include <meave/lib/utils.hpp>

namespace meave { namespace rothash {

namespace aux {

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
	static ::uint32_t rol(const ::uint32_t x) noexcept {
		return (x << ROL_BITS) | (x >> (sizeof(x)*8 - ROL_BITS));
	}

	static ::uint32_t hash_aligned(const ::uint8_t *p, ::size_t len) noexcept {
		::uint32_t h[8] = {};

		const ::size_t L = len;
		if (len >= 32) {
			do {
				const ::uint32_t *u = reinterpret_cast<const ::uint32_t*>(&p[L - len]);
				h[0] = rol(h[0]) ^ u[0];
				h[1] = rol(h[1]) ^ u[1];
				h[2] = rol(h[2]) ^ u[2];
				h[3] = rol(h[3]) ^ u[3];
				h[4] = rol(h[4]) ^ u[4];
				h[5] = rol(h[5]) ^ u[5];
				h[6] = rol(h[6]) ^ u[6];
				h[7] = rol(h[7]) ^ u[7];
			} while ((len -= 32) >= 32);
			h[0] = h[0] ^ h[4];
			h[1] = h[1] ^ h[5];
			h[2] = h[2] ^ h[6];
			h[3] = h[3] ^ h[7];
		}
		if (len >= 16) {
			const ::uint32_t *u = reinterpret_cast<const unsigned*>(&p[L - len]);
			h[0] = rol(h[0]) ^ u[0];
			h[1] = rol(h[1]) ^ u[1];
			h[2] = rol(h[2]) ^ u[2];
			h[3] = rol(h[3]) ^ u[3];
			h[0] = h[0] ^ h[2];
			h[1] = h[1] ^ h[3];
			len -= 16;
		}
		if (len >= 8) {
			const ::uint32_t *u = reinterpret_cast<const unsigned*>(&p[L - len]);
			h[0] = rol(h[0]) ^ u[0];
			h[1] = rol(h[1]) ^ u[1];
			h[0] = h[0] ^ h[1];
			len -= 8;
		}
		if (len >= 4) {
			const ::uint32_t *u = reinterpret_cast<const unsigned*>(&p[L - len]);
			h[0] = rol(h[0]) ^ u[0];
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
		return h[0];
	}

	static unsigned hash_unaligned(const ::uint8_t *p, const ::size_t len) noexcept {
		unsigned hash = 0;
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

} /* namespace aux */

template <unsigned ROL_BITS>
unsigned naive(const ::uint8_t *p, const ::size_t len) noexcept {
	return aux::HashFunc<ROL_BITS>::hash(p, len);
}

} } /* meave::rothash */

#endif // MEAVE_LIB_ROTHASH_NAIVE_HPP
