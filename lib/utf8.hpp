#ifndef MEAVE_LIB_HPP
#   define MEAVE_LIB_HPP

#   include <boost/assert.hpp>

namespace meave {
    enum { UNI_REPLACEMENT = 0xfffc, };

    template <typename UInt>
    bool utf8_get_next(const char c, UInt &u) {
        unsigned char b = static_cast<unsigned char>(c);
        if ((b & 0xc0) != 0x80) {
            return false;
        } else {
            u = (u << 6) | (b & 0x3f);
            return true;
        }
    }


    // Note: it is possible that len < 0 , it means, that the first character (s[-1]) is wrong
    template <typename It, typename UInt>
    It utf8_get_all_next(It it, const ssize_t len, UInt &u, const UInt def_u) {
        ssize_t i;
        for (i = 0; i < len; ++i) {
            if (utf8_get_next(*it, u)) {
                ++it;
            } else {
                break;
            }
        }
        if (i != len) { // either len < 0 or some utf8_get_next(.) returned false
            u = def_u;
        }
        return it;
    }

    static ssize_t utf8_to_u16_helper(uint16_t &u) {
        if (u < 0x80) {
            return 0;
        } else if (u < 0xc0) {
           /* Incorrect byte sequence */
            return -1;
        } else if (u < 0xe0) {
            u &= 0x1f;
            return 1;
        } else if (u < 0xf0) {
            u &= 0x0f;
            return 2;
        } else {
            return -1;
        }
    }

    static ssize_t utf8_to_u32_helper(uint32_t &u) {
        if (u < 0x80) {
            return 0;
        } else if (u < 0xc0) {
            /* Incorrect byte sequence */
            return -1;
        } else if (u < 0xe0) {
            u &= 0x1f;
            return 1;
        } else if (u < 0xf0) {
            u &= 0x0f;
            return 2;
        } else if (u < 0xf8) {
            u &= 0x07;
            return 3;
        } else if (u < 0xfc) {
            u &= 0x03;
            return 4;
        } else if (u < 0xfe) {
            u &= 0x01;
            return 5;
        } else {
            return -1;
        }
    }

    template<typename It>
    static It utf8_to_u16(It it, uint16_t &u, const uint16_t def_u = UNI_REPLACEMENT) {
        const ssize_t len = utf8_to_u16_helper(u = static_cast<unsigned char>(*it));
        return utf8_get_all_next(++it, len, u, def_u);
    }

    template<typename It>
    static const char *utf8_to_u32(It it, uint32_t &u, const uint32_t def_u = UNI_REPLACEMENT) {
        const ssize_t len = utf8_to_u32_helper(u = static_cast<unsigned char>(*it));
        return utf8_get_all_next(++it, len, u, def_u);
    }

    template<size_t S>
    struct UInt { };

    template<>
    struct UInt<2> {
        typedef uint16_t T;
    };

    template<>
    struct UInt<4> {
        typedef uint32_t T;
    };

    template<size_t S>
    struct UTF8_TO_X { };

    template<>
    struct UTF8_TO_X<2> {
        template<typename It, typename UInt>
        It operator()(It it, UInt &u, const UInt def_u) {
            return utf8_to_u16(it, u, def_u);
        }
    };

    template<>
    struct UTF8_TO_X<4> {
        template<typename It, typename UInt>
        It operator()(It it, UInt &u, const UInt def_u) {
            return utf8_to_u32(it, u, def_u);
        }
    };

    template<typename It>
    static It utf8_to_wchar(It it, wchar_t &wch, const wchar_t def_wch = UNI_REPLACEMENT) {
        typedef UInt<sizeof(wch)>::T UInt;
        UInt u;
        const UInt def_u = static_cast<UInt>(def_wch);
        It ret = UTF8_TO_X<sizeof(wch)>()(it, u, def_u);
        wch = static_cast<wchar_t>(u);
        return ret;
    }

    template <typename It, typename UInt>
    It utf8_put_all_next(It it, const ssize_t len, UInt u) {
        BOOST_VERIFY(len >= 0 && len < 6);
        for (ssize_t i = 0; i < len; ++i) {
            const unsigned shift = (static_cast<unsigned>(len - i) - 1) * 6;
            *it++ = 0x80 | ((u >> shift) & 0x3f);
        }
        return it;
    }

    static ssize_t u16_to_utf8_helper(char &dst, const uint16_t u) {
        if (u < 0x80) {
            dst = static_cast<unsigned char>(u);
            return 0;
        } else if (u < 0x800) {
            dst = static_cast<unsigned char>(0xc0 | (u >> 6));
            return 1;
        } else if (u < 0x10000) {
            dst = static_cast<unsigned char>(0x80 | (u & 0x3f));
            return 2;
        } else {
            BOOST_VERIFY(0);
        }
    }

    static ssize_t u32_to_utf8_helper(char &dst, const uint16_t u) {
        if (u < 0x80) {
            dst = static_cast<unsigned char>(u);
            return 0;
        } else if (u < 0x800) {
            dst = static_cast<unsigned char>(0xc0 | (u >> 6));
            return 1;
        } else if (u < (1<<16)) {
            dst = static_cast<unsigned char>(0xe0 | (u >> 12));
            return 2;
        } else if (u < (1<<21)) {
            dst = static_cast<unsigned char>(0xf0 | (u >> 18));
            return 3;
        } else if (u < (1<<26)) {
            dst = static_cast<unsigned char>(0xf8 | (u >> 24));
            return 4;
        } else if (u < (1U<<31)) {
            dst = static_cast<unsigned char>(0xfc | (u >> 30));
            return 5;
        } else {
            BOOST_VERIFY(0);
        }
    }

    template<typename It>
    static It u16_to_utf8(It it_dst, const uint16_t u) {
        const ssize_t len = u16_to_utf8_helper(*it_dst, u);
        return utf8_put_all_next(++it_dst, len, u);
    }

    template<typename It>
    static It u32_to_utf8(It it_dst, const uint32_t u) {
        const ssize_t len = u32_to_utf8_helper(*it_dst, u);
        return utf8_put_all_next(++it_dst, len, u);
    }

    template<size_t S>
    struct X_TO_UTF8 { };

    template<>
    struct X_TO_UTF8<2> {
        template <typename It, typename Ch>
        It operator()(It it_dst, const Ch c) const {
            return u16_to_utf8(it_dst, static_cast<uint16_t>(c));
        }
    };

    template<>
    struct X_TO_UTF8<4> {
        template <typename It, typename Ch>
        It operator()(It it_dst, const Ch c) const {
            return u32_to_utf8(it_dst, static_cast<uint32_t>(c));
        }
    };

    template<typename It>
    static It wchar_to_utf8(It it_dst, const wchar_t wch) {
        return X_TO_UTF8<sizeof(wch)>()(it_dst, wch);
    }
}

#endif
