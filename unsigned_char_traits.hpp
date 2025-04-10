// unsigned_char_traits.hpp
#pragma once

#include <string>
#include <cstring>

namespace std {
    template <>
    struct char_traits<unsigned int> {
        using char_type = unsigned int;
        using int_type = unsigned int;
        using pos_type = size_t;
        using off_type = ptrdiff_t;

        static void assign(char_type& c1, const char_type& c2) { c1 = c2; }
        static bool eq(char_type c1, char_type c2) { return c1 == c2; }
        static bool lt(char_type c1, char_type c2) { return c1 < c2; }

        static int compare(const char_type* s1, const char_type* s2, size_t n) {
            for (size_t i = 0; i < n; ++i) {
                if (lt(s1[i], s2[i])) return -1;
                if (lt(s2[i], s1[i])) return 1;
            }
            return 0;
        }

        static size_t length(const char_type* s) {
            size_t len = 0;
            while (s[len]) ++len;
            return len;
        }

        static const char_type* find(const char_type* s, size_t n, const char_type& a) {
            for (size_t i = 0; i < n; ++i)
                if (eq(s[i], a)) return s + i;
            return nullptr;
        }

        static char_type* move(char_type* s1, const char_type* s2, size_t n) {
            return (char_type*)memmove(s1, s2, n * sizeof(char_type));
        }

        static char_type* copy(char_type* s1, const char_type* s2, size_t n) {
            return (char_type*)memcpy(s1, s2, n * sizeof(char_type));
        }

        static char_type* assign(char_type* s, size_t n, char_type a) {
            for (size_t i = 0; i < n; ++i) s[i] = a;
            return s;
        }

        static int_type not_eof(int_type c) { return c != 0 ? c : 1; }
        static char_type to_char_type(int_type c) { return c; }
        static int_type to_int_type(char_type c) { return c; }
        static bool eq_int_type(int_type c1, int_type c2) { return c1 == c2; }
        static int_type eof() { return 0; }
    };
}
