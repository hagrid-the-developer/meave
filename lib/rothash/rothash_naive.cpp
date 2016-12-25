#include <cstddef>
#include <cstdint>

#include <meave/lib/utils.hpp>

namespace meave { namespace rothash {

namespace aux {

template<typename T = uns>
class HashBase {
protected:
	static const struct MaskBits {
		uns bits[sizeof(T)];

		MaskBits() {
			u8 *p = reinterpret_cast<u8*>(bits);
			for (::size_t i = 0; i < sizeof(T); ++i) {
				for (::size_t j = 0; j < sizeof(T); ++j) {
					p[i*sizeof(T) + j] = j < i ? 0xFF : 0;
				}
			}
		}
	} mask;

	static uns unaligned_part(const u8 *x) noexcept {
		return static_cast<T>( reinterpret_cast< ::uintptr_t>(x) % sizeof(T) );
	}
};
template<typename T>
const typename HashBase<T>::MaskBits HashBase<T>::mask;

template <uns ROL_BITS>
class HashFunc : public aux::HashBase<uns> {
private:
	static uns rol(const uns x) noexcept {
		return (x << ROL_BITS) | (x >> (sizeof(x)*8 - ROL_BITS));
	}

	static uns hash_aligned(const u8 *p, const ::size_t len, uns hash) noexcept {
		const uns *u = reinterpret_cast<const uns*>(p);
		::size_t l = len;
		for (; l >= sizeof(uns); l -= sizeof(uns)) {
			hash = rol(hash) ^ *u++;
		}
		hash = rol(hash) ^ (*u & mask.bits[l]);
		return hash;
	}

	static uns hash_unaligned(const u8 *p, const ::size_t len, uns hash) noexcept {
		for (uns i = 0; ; ++i) {
			const ::size_t mod = i % sizeof(uns);
			if (0 == mod)
				hash = rol(hash);

			if (i >= len)
				break;

#			if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			const ::size_t shift = 8*mod;
#			elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
			const ::size_t shift = 8*(sizeof(uns) - mod - 1);
#			else
#			error Sorry, cannot determine endiadness
#			endif

			hash ^= p[i] << shift;
		}
		return hash;
	}

public:
	static uns hash(const u8 *p, const ::size_t len) noexcept {
		const auto alignment = unaligned_part(p);
		if (0 == alignment)
			return hash_aligned(p, len);

		return hash_unaligned(p, len);
	}
};

} /* namespace aux */

template <typename uns ROL_BITS>
uns naive(const u8 *p, const ::size_t len) noexcept {
	return HashFunc<ROL_BITS>::hash(p, len);
}

} } /* meave::rothash */
