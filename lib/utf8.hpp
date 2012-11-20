#ifndef MEAVE_LIB_HPP
#   define MEAVE_LIB_HPP

#   include <boost/assert.hpp>

namespace meave {
    enum { UNI_REPLACEMENT = 0xfffc, };

    template <typename UInt>
    bool utf8_get_next(const char *s, UInt &u) {
        unsigned char b = static_cast<unsigned char>(*s);
        if ((b & 0xc0) != 0x80) {
            return false;
        } else {
            u = (u << 6) | (b & 0x3f);
            return true;
        }
    }


    // Note: it is possible that len < 0 , it means, that the first character (s[-1]) is wrong
    template <typename UInt>
    const char *utf8_get_all_next(const char *s, const ssize_t len, UInt &u, const UInt def_u) {
        ssize_t i;
        for (i = 0; i < len; ++i) {
            if (utf8_get_next(s, u)) {
                ++s;
            } else {
                break;
            }
        }
        if (i != len) { // either len < 0 or some utf8_get_next(.) returned false
            u = def_u;
        }
        return s;
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

    static const char *utf8_to_u16(const char *s, uint16_t &u, const uint16_t def_u = UNI_REPLACEMENT) {
        const ssize_t len = utf8_to_u16_helper(u = static_cast<unsigned char>(*s++));
        return utf8_get_all_next(s, len, u, def_u);
    }

    static const char *utf8_to_u32(const char *s, uint32_t &u, const uint32_t def_u = UNI_REPLACEMENT) {
        const ssize_t len = utf8_to_u32_helper(u = static_cast<unsigned char>(*s++));
        return utf8_get_all_next(s, len, u, def_u);
    }

    static const char *utf8_to_wchar(const char *s, wchar_t &wch, const wchar_t def_wch = UNI_REPLACEMENT) {
        if (sizeof(wchar_t) == 2) {
            uint16_t u;
            const char *ret = utf8_to_u16(s, u, def_wch);
            wch = static_cast<wchar_t>(u);
            return ret;
        }
        if (sizeof(wchar_t) == 4) {
            uint32_t u;
            const char *ret = utf8_to_u32(s, u, def_wch);
            wch = static_cast<wchar_t>(u);
            return ret;
        }
        BOOST_VERIFY(0);
    }

    template <typename UInt>
    char *utf8_put_all_next(char *s, const ssize_t len, UInt u) {
        BOOST_VERIFY(len >= 0 && len < 6);
        for (ssize_t i = 0; i < len; ++i) {
            const unsigned shift = (static_cast<unsigned>(len) - i - 1) * 6;
            *s++ = 0x80 | ((u >> shift) & 0x3f);
        }
        return s;
    }

    static ssize_t u16_to_utf8_helper(char *dst, const uint16_t u) {
        if (u < 0x80) {
            *dst = static_cast<unsigned char>(u);
            return 0;
        } else if (u < 0x800) {
            *dst = static_cast<unsigned char>(0xc0 | (u >> 6));
            return 1;
        } else if (u < 0x10000) {
            *dst = 0x80 | (u & 0x3f);
            return 2;
        } else {
            BOOST_VERIFY(0);
        }
    }

    static ssize_t u32_to_utf8_helper(char *dst, const uint16_t u) {
        if (u < 0x80) {
            *dst = static_cast<unsigned char>(u);
            return 0;
        } else if (u < 0x800) {
            *dst = static_cast<unsigned char>(0xc0 | (u >> 6));
            return 1;
        } else if (u < (1<<16)) {
            *dst = static_cast<unsigned char>(0xe0 | (u >> 12));
            return 2;
        } else if (u < (1<<21)) {
            *dst = static_cast<unsigned char>(0xf0 | (u >> 18));
            return 3;
        } else if (u < (1<<26)) {
            *dst = static_cast<unsigned char>(0xf8 | (u >> 24));
            return 4;
        } else if (u < (1U<<31)) {
            *dst = static_cast<unsigned char>(0xfc | (u >> 30));
            return 5;
        } else {
            BOOST_VERIFY(0);
        }
    }

    static char *u16_to_utf8(char *dst, const uint16_t u) {
        const ssize_t len = u16_to_utf8_helper(dst++, u);
        return utf8_put_all_next(dst, len, u);
    }

    static char *u32_to_utf8(char *dst, const uint32_t u) {
        const ssize_t len = u32_to_utf8_helper(dst++, u);
        return utf8_put_all_next(dst, len, u);
    }

    static char *wchar_to_utf8(char *dst, const wchar_t wch) {
        if (sizeof(wchar_t) == 2) {
            return u16_to_utf8(dst, static_cast<uint16_t>(wch));
        }
        if (sizeof(wchar_t) == 4) {
            return u32_to_utf8(dst, static_cast<uint32_t>(wch));
        }
        BOOST_VERIFY(0);
    }
}

#endif
