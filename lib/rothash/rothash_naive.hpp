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
	static unsigned rol(const unsigned x) noexcept {
		return (x << ROL_BITS) | (x >> (sizeof(x)*8 - ROL_BITS));
	}

	static unsigned hash_aligned(const ::uint8_t *p, const ::size_t len) noexcept {
		unsigned hash = 0;
		const unsigned *u = reinterpret_cast<const unsigned*>(p);
		::size_t l = len;
		for (; l >= sizeof(unsigned); l -= sizeof(unsigned)) {
			hash = rol(hash) ^ *u++;
		}
		hash = rol(hash) ^ (*u & mask.bits[l]);
		return hash;
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
