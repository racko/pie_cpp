#pragma once

#include "pie_base.h"
#include <ostream>

struct Atom_t : Pie<Atom_t> {
    int height_{};
};

inline constexpr Atom_t Atom;

constexpr bool equal(Atom_t, Atom_t) { return true; }

inline void print(std::ostream& s, Atom_t) { s << "Atom"; }

struct Quote_t : Pie<Quote_t> {
    constexpr Quote_t(const char* symbol) : symbol_{symbol} {}
    int height_{};
    const char* symbol_;
};

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

constexpr bool equal(const Quote_t lhs, const Quote_t rhs) { return detail::strcmp(lhs.symbol_, rhs.symbol_) == 0; }

inline void print(std::ostream& s, const Quote_t atom) { s << '\'' << atom.symbol_; }

constexpr Quote_t quote(const char* symbol) { return Quote_t(symbol); }

constexpr bool IsA1(const Quote_t atom, Atom_t) {
    if (*atom.symbol_ == '\0') {
        return false;
    }
    auto c = atom.symbol_;
    do {
        if (!detail::isalpha(*c) && *c != '-') {
            return false;
        }
        ++c;
    } while (*c != '\0');
    return true;
}

constexpr bool IsAType1(Atom_t) { return true; }

template <>
struct is_normal<Atom_t> : std::true_type {};

template <>
struct is_value<Atom_t> : std::true_type {};

template <>
struct is_normal<Quote_t> : std::true_type {};

template <>
struct is_value<Quote_t> : std::true_type {};

template <>
struct synth_result<Atom_t> {
    using type = U_t;
};

constexpr synth_result_t<Atom_t> synth1(Atom_t) { return U; }

template <>
struct synth_result<Quote_t> {
    using type = Atom_t;
};

constexpr synth_result_t<Quote_t> synth1(const Quote_t atom) {
    assert(IsA(atom, Atom));
    return Atom;
}
