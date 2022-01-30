#include "strpp.h"
#include <cassert>
#include <cctype>
#include <iterator>

// https://stackoverflow.com/a/28139075
template <typename T>
struct reversion_wrapper { T& iterable; };

template <typename T>
auto begin (reversion_wrapper<T> w) { return std::rbegin(w.iterable); }

template <typename T>
auto end (reversion_wrapper<T> w) { return std::rend(w.iterable); }

template <typename T>
reversion_wrapper<T> reversed (T&& iterable) { return { iterable }; }

std::string operator++(std::string s)
{
    auto       it  = s.begin();
    auto const end = s.end();
    for (; it != end; ++it) {
        if (!std::isalpha(*it))
            break;
    }

    auto const alphas = std::distance(s.begin(), it);
    auto const endalpha = it;

    for (; it != end; ++it) {
        if (!std::isdigit(*it))
            break;
    }

    auto const digits = std::distance(endalpha, it);
    auto const rest   = std::distance(it, end);

    if (   (alphas == 0 and digits == 0)
        or (rest != 0)
    )
    {
        return s;
    }

    auto incrementer = [] (char& ch, char cy) -> char {
        //ch += cy + 1;
        //ch %=
        switch (ch) {
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
        case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
            ch = ch - 'a' + cy; // not an ASCII value
            cy = ch / 26;
            ch = ch % 26 + 'a'; // now it is
            break;

        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M':
        case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
            ch = ch - 'A' + cy; // not an ASCII value
            cy = ch / 26;
            ch = ch % 26 + 'A'; // now it is
            break;

        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            ch = ch - '0' + cy; // not an ASCII value
            cy = ch / 10;
            ch = ch % 10 + '0'; // now it is
            break;

        default:
            assert(!"Impossible character");
        }
        return cy;
    };

    char carry = 1;
    for (char & c : reversed(s)) { // we need a reference into the string
        carry = incrementer(c, carry);
    }

    if (carry) {
        s.insert(0, 1, s.front() > '9' ? 'a' : '1');
    }
    return s;
}
