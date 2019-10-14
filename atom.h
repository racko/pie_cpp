#pragma once

#include "pie_base.h"
#include <ostream>
#include <type_traits>

struct Atom_t : Pie<Atom_t> {};

template <>
struct Height<Atom_t> : std::integral_constant<int, 0> {};

inline constexpr Atom_t Atom;

template <>
struct Equal<Atom_t, Atom_t> : std::true_type {};

template <>
struct Printer<Atom_t> {
    static void print(std::ostream& s) { s << "Atom"; }
};

template <typename Symbol>
struct Quote_t : Pie<Quote_t<Symbol>> {
    constexpr Quote_t() = default;
};

template <typename Symbol>
struct Height<Quote_t<Symbol>> : std::integral_constant<int, 0> {};

namespace detail {
constexpr int strcmp(const char* lhs, const char* rhs) {
    while (true) {
        if (*lhs < *rhs) {
            return -1;
        }
        if (*lhs > *rhs) {
            return 1;
        }
        if (*lhs == '\0') {
            return 0;
        }
        ++lhs;
        ++rhs;
    }
}

constexpr bool isalpha(int c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }

} // namespace detail

template <typename Symbol1, typename Symbol2>
struct Equal<Quote_t<Symbol1>, Quote_t<Symbol2>>
    : std::bool_constant<detail::strcmp(Symbol1::value, Symbol2::value) == 0> {};

template <typename Symbol>
struct Printer<Quote_t<Symbol>> {
    static void print(std::ostream& s) { s << '\'' << Symbol::value; }
};

template <typename Symbol>
constexpr Quote_t<Symbol> quote() {
    return Quote_t<Symbol>{};
}

namespace detail {
template <typename Symbol>
constexpr bool IsASymbol() {
    if (*Symbol::value == '\0') {
        return false;
    }
    auto c = Symbol::value;
    do {
        if (!detail::isalpha(*c) && *c != '-') {
            return false;
        }
        ++c;
    } while (*c != '\0');
    return true;
}
}

template <>
struct is_a_type_impl<Atom_t> : std::true_type {};

template <typename Symbol>
struct is_a_impl<Quote_t<Symbol>, Atom_t> : std::bool_constant<detail::IsASymbol<Symbol>()> {};

template <>
struct is_normal<Atom_t> : std::true_type {};

template <>
struct is_value<Atom_t> : std::true_type {};

template <typename Symbol>
struct is_normal<Quote_t<Symbol>> : std::true_type {};

template <typename Symbol>
struct is_value<Quote_t<Symbol>> : std::true_type {};

template<>
struct Synth<Atom_t> {
    using type = U_t;
};

template<typename Symbol>
struct Synth<Quote_t<Symbol>> {
    static_assert(is_a_v<Quote_t<Symbol>, Atom_t>);
    using type = Atom_t;
};
