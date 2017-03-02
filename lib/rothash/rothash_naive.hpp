#ifndef MEAVE_LIB_ROTHASH_NAIVE_HPP
#	define MEAVE_LIB_ROTHASH_NAIVE_HPP

#include <cstddef>
#include <cstdint>

#include <meave/lib/utils.hpp>

namespace meave { namespace rothash {

namespace aux {

template<typename T = ::uint32_t>
class HashBaseForBasic {
protected:
	static const struct MaskBits {
		::uint32_t bits[sizeof(T)];

		MaskBits() {
			::uint8_t *p = reinterpret_cast<::uint8_t*>(bits);
			for (::size_t i = 0; i < sizeof(T); ++i) {
				for (::size_t j = 0; j < sizeof(T); ++j) {
					p[i*sizeof(T) + j] = j < i ? 0xFF : 0;
				}
			}
		}
	} mask;

	static ::size_t unaligned_part(const ::uint8_t *x) noexcept {
		return reinterpret_cast< ::uintptr_t>(x) % sizeof(T);
	}
};
template<typename T>
const typename HashBaseForBasic<T>::MaskBits HashBaseForBasic<T>::mask;

template <unsigned ROL_BITS>
class HashFuncForBasic : public aux::HashBaseForBasic<> {
private:
	static ::uint32_t rol(const ::uint32_t x) noexcept {
		return (x << ROL_BITS) | (x >> (sizeof(x)*8 - ROL_BITS));
	}

	static ::uint32_t hash_aligned(const ::uint8_t *p, const ::size_t len, ::uint32_t hash) noexcept {
		const ::uint32_t *u = reinterpret_cast<const ::uint32_t*>(p);
		::size_t l = len;
		for (; l >= sizeof(::uint32_t); l -= sizeof(::uint32_t)) {
			hash = rol(hash) ^ *u++;
		}
		hash = rol(hash) ^ (*u & mask.bits[l]);
		return hash;
	}

	static ::uint32_t hash_unaligned(const ::uint8_t *p, const ::size_t len, ::uint32_t hash) noexcept {
		for (unsigned i = 0; ; ++i) {
			const ::size_t mod = i % sizeof(::uint32_t);
			if (0 == mod)
				hash = rol(hash);

			if (i >= len)
				break;

#			if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			const ::size_t shift = 8*mod;
#			elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
			const ::size_t shift = 8*(sizeof(::uint32_t) - mod - 1);
#			else
#			error Sorry, cannot determine endiadness
#			endif

			hash ^= p[i] << shift;
		}
		return hash;
	}

public:
	static ::uint32_t hash(const ::uint8_t *p, const ::size_t len, const ::uint32_t hash = 0) noexcept {
		const auto alignment = unaligned_part(p);
		if (__builtin_expect(!alignment, 1))
			return hash_aligned(p, len, hash);

		return hash_unaligned(p, len, hash);
	}
};

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
			len -= 16;
		}
		h[0] = h[0] ^ h[2];
		h[1] = h[1] ^ h[3];
		if (len >= 8) {
			const ::uint32_t *u = reinterpret_cast<const unsigned*>(&p[L - len]);
			h[0] = rol(h[0]) ^ u[0];
			h[1] = rol(h[1]) ^ u[1];
			len -= 8;
		}
		h[0] = h[0] ^ h[1];
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

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
class HashFunc2 : public aux::HashBase<unsigned> {
private:
	template <unsigned ROL_BITS>
	static ::uint32_t rol(const ::uint32_t x) noexcept {
		return (x << ROL_BITS) | (x >> (sizeof(x)*8 - ROL_BITS));
	}

	static ::uint64_t hash_aligned(const ::uint8_t *p, ::size_t len) noexcept {
		::uint32_t h0[8] = {};
		::uint32_t h1[8] = {};

		const ::size_t L = len;
		if (len >= 32) {
			do {
				const ::uint32_t *u = reinterpret_cast<const ::uint32_t*>(&p[L - len]);
				h0[0] = rol<ROL_BITS0>(h0[0]) ^ u[0];
				h0[1] = rol<ROL_BITS0>(h0[1]) ^ u[1];
				h0[2] = rol<ROL_BITS0>(h0[2]) ^ u[2];
				h0[3] = rol<ROL_BITS0>(h0[3]) ^ u[3];
				h0[4] = rol<ROL_BITS0>(h0[4]) ^ u[4];
				h0[5] = rol<ROL_BITS0>(h0[5]) ^ u[5];
				h0[6] = rol<ROL_BITS0>(h0[6]) ^ u[6];
				h0[7] = rol<ROL_BITS0>(h0[7]) ^ u[7];
				h1[0] = rol<ROL_BITS1>(h1[0]) ^ u[0];
				h1[1] = rol<ROL_BITS1>(h1[1]) ^ u[1];
				h1[2] = rol<ROL_BITS1>(h1[2]) ^ u[2];
				h1[3] = rol<ROL_BITS1>(h1[3]) ^ u[3];
				h1[4] = rol<ROL_BITS1>(h1[4]) ^ u[4];
				h1[5] = rol<ROL_BITS1>(h1[5]) ^ u[5];
				h1[6] = rol<ROL_BITS1>(h1[6]) ^ u[6];
				h1[7] = rol<ROL_BITS1>(h1[7]) ^ u[7];
			} while ((len -= 32) >= 32);
			h0[0] = h0[0] ^ h0[4];
			h0[1] = h0[1] ^ h0[5];
			h0[2] = h0[2] ^ h0[6];
			h0[3] = h0[3] ^ h0[7];
			h1[0] = h1[0] ^ h1[4];
			h1[1] = h1[1] ^ h1[5];
			h1[2] = h1[2] ^ h1[6];
			h1[3] = h1[3] ^ h1[7];
		}
		if (len >= 16) {
			const ::uint32_t *u = reinterpret_cast<const unsigned*>(&p[L - len]);
			h0[0] = rol<ROL_BITS0>(h0[0]) ^ u[0];
			h0[1] = rol<ROL_BITS0>(h0[1]) ^ u[1];
			h0[2] = rol<ROL_BITS0>(h0[2]) ^ u[2];
			h0[3] = rol<ROL_BITS0>(h0[3]) ^ u[3];
			h1[0] = rol<ROL_BITS1>(h1[0]) ^ u[0];
			h1[1] = rol<ROL_BITS1>(h1[1]) ^ u[1];
			h1[2] = rol<ROL_BITS1>(h1[2]) ^ u[2];
			h1[3] = rol<ROL_BITS1>(h1[3]) ^ u[3];
			len -= 16;
		}
		h0[0] = h0[0] ^ h0[2];
		h0[1] = h0[1] ^ h0[3];
		h1[0] = h1[0] ^ h1[2];
		h1[1] = h1[1] ^ h1[3];
		if (len >= 8) {
			const ::uint32_t *u = reinterpret_cast<const unsigned*>(&p[L - len]);
			h0[0] = rol<ROL_BITS0>(h0[0]) ^ u[0];
			h0[1] = rol<ROL_BITS0>(h0[1]) ^ u[1];
			h1[0] = rol<ROL_BITS1>(h1[0]) ^ u[0];
			h1[1] = rol<ROL_BITS1>(h1[1]) ^ u[1];
			len -= 8;
		}
		if (len >= 4) {
			const ::uint32_t *u = reinterpret_cast<const unsigned*>(&p[L - len]);
			h0[0] = rol<ROL_BITS0>(h0[0]) ^ u[0];
			h1[0] = rol<ROL_BITS1>(h1[0]) ^ u[0];
			len -= 4;
		}
		h0[0] = h0[0] ^ h0[1];
		h1[0] = h1[0] ^ h1[1];
		assert(!len);

		return h0[0] | ::uint64_t(h1[0]) << 32;
	}

public:
	static ::uint64_t hash(const ::uint8_t *p, const ::size_t len) noexcept {
		return hash_aligned(p, len);
	}
};

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
class HashFunc3 : public aux::HashBase<unsigned> {
private:
	template <unsigned ROL_BITS>
	static ::uint32_t rol(const ::uint32_t x) noexcept {
		return (x << ROL_BITS) | (x >> (sizeof(x)*8 - ROL_BITS));
	}

	static ::uint64_t hash_aligned(const ::uint8_t *p, ::size_t len) noexcept {
		::uint64_t h0[4] = {};
		::uint64_t h1[4] = {};

		const ::size_t L = len;
		if (len >= 16) {
			do {
				const ::uint32_t *u = reinterpret_cast<const ::uint32_t*>(&p[L - len]);
				h0[0] = rol<ROL_BITS0>(h0[0]) ^ u[0];
				h0[1] = rol<ROL_BITS0>(h0[1]) ^ u[1];
				h0[2] = rol<ROL_BITS0>(h0[2]) ^ u[2];
				h0[3] = rol<ROL_BITS0>(h0[3]) ^ u[3];
				h1[0] = rol<ROL_BITS1>(h1[0]) ^ u[0];
				h1[1] = rol<ROL_BITS1>(h1[1]) ^ u[1];
				h1[2] = rol<ROL_BITS1>(h1[2]) ^ u[2];
				h1[3] = rol<ROL_BITS1>(h1[3]) ^ u[3];
			} while ((len -= 16) >= 16);
			h0[0] = h0[0] ^ h0[2];
			h0[1] = h0[1] ^ h0[3];
			h1[0] = h1[0] ^ h1[2];
			h1[1] = h1[1] ^ h1[3];
		}
		const ::uint32_t *u = reinterpret_cast<const ::uint32_t*>(&p[L - len]);
		switch (len) {
		case 12:
			h0[0] = rol<ROL_BITS0>(h0[0]) ^ u[2];
			h1[0] = rol<ROL_BITS1>(h1[0]) ^ u[2];
		case 8:
			h0[1] = rol<ROL_BITS0>(h0[1]) ^ u[1];
			h1[1] = rol<ROL_BITS1>(h1[1]) ^ u[1];
		case 4:
			h0[0] = rol<ROL_BITS0>(h0[0]) ^ u[0];
			h1[0] = rol<ROL_BITS1>(h1[0]) ^ u[0];
		case 0:
			break;
		default:
			__builtin_unreachable();
		}

		h0[0] = h0[0] ^ h0[1];
		h1[0] = h1[0] ^ h1[1];
		assert(!len);

		return h0[0] | h1[0] << 32;
	}

public:
	static ::uint64_t hash(const ::uint8_t *p, const ::size_t len) noexcept {
		return hash_aligned(p, len);
	}
};

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
class HashFunc4 : public aux::HashBase<unsigned> {
public:
	struct Result {
		::uint32_t _[4];
	};

private:
	template <unsigned ROL_BITS>
	static ::uint32_t rol(const ::uint32_t x) noexcept {
		return (x << ROL_BITS) | (x >> (sizeof(x)*8 - ROL_BITS));
	}

	static Result hash_aligned(const ::uint8_t *p, ::size_t len) noexcept {
		Result h0 = {};
		Result h1 = {};

		const ::size_t L = len;
		if (len >= 16) {
			do {
				const ::uint32_t *u = reinterpret_cast<const ::uint32_t*>(&p[L - len]);
				h0._[0] = rol<ROL_BITS0>(h0._[0]) ^ u[0];
				h0._[1] = rol<ROL_BITS0>(h0._[1]) ^ u[1];
				h0._[2] = rol<ROL_BITS0>(h0._[2]) ^ u[2];
				h0._[3] = rol<ROL_BITS0>(h0._[3]) ^ u[3];
				h1._[0] = rol<ROL_BITS1>(h1._[0]) ^ u[0];
				h1._[1] = rol<ROL_BITS1>(h1._[1]) ^ u[1];
				h1._[2] = rol<ROL_BITS1>(h1._[2]) ^ u[2];
				h1._[3] = rol<ROL_BITS1>(h1._[3]) ^ u[3];
			} while ((len -= 16) >= 16);
		}
		if (len >= 8) {
			const ::uint32_t *u = reinterpret_cast<const unsigned*>(&p[L - len]);
			h0._[0] = rol<ROL_BITS0>(h0._[0]) ^ u[0];
			h0._[1] = rol<ROL_BITS0>(h0._[1]) ^ u[1];
			h1._[0] = rol<ROL_BITS1>(h1._[0]) ^ u[0];
			h1._[1] = rol<ROL_BITS1>(h1._[1]) ^ u[1];
			len -= 8;
		}
		if (len >= 4) {
			const ::uint32_t *u = reinterpret_cast<const unsigned*>(&p[L - len]);
			h0._[0] = rol<ROL_BITS0>(h0._[0]) ^ u[0];
			h1._[0] = rol<ROL_BITS1>(h1._[0]) ^ u[0];
			len -= 4;
		}
		assert(!len);

		return Result{ h0._[0] ^ h0._[2], h0._[1] ^ h0._[3], h1._[0] ^ h1._[2], h1._[1] ^ h1._[3] };
	}

public:
	static Result hash(const ::uint8_t *p, const ::size_t len) noexcept {
		return hash_aligned(p, len);
	}
};

} /* namespace aux */

template <unsigned ROL_BITS>
unsigned basic(const ::uint8_t *p, const ::size_t len) noexcept {
	return aux::HashFuncForBasic<ROL_BITS>::hash(p, len);
}

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
::uint64_t basic_duo(const ::uint8_t *p, const ::size_t len) noexcept {
	 return ::uint64_t(aux::HashFuncForBasic<ROL_BITS0>::hash(p, len)) << 32 | aux::HashFuncForBasic<ROL_BITS1>::hash(p, len);
}

template <unsigned ROL_BITS>
unsigned naive(const ::uint8_t *p, const ::size_t len) noexcept {
	return aux::HashFunc<ROL_BITS>::hash(p, len);
}

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
::uint64_t naive2(const ::uint8_t *p, const ::size_t len) noexcept {
	return aux::HashFunc2<ROL_BITS0, ROL_BITS1>::hash(p, len);
}

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
::uint64_t naive3(const ::uint8_t *p, const ::size_t len) noexcept {
	return aux::HashFunc3<ROL_BITS0, ROL_BITS1>::hash(p, len);
}

template <unsigned ROL_BITS0, unsigned ROL_BITS1>
typename aux::HashFunc4<ROL_BITS0, ROL_BITS1>::Result naive4(const ::uint8_t *p, const ::size_t len) noexcept {
	return aux::HashFunc4<ROL_BITS0, ROL_BITS1>::hash(p, len);
}

} } /* meave::rothash */

#endif // MEAVE_LIB_ROTHASH_NAIVE_HPP
