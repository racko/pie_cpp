#pragma once

#include "pie_base.h"
#include <ostream>

struct Atom_t : Pie<Atom_t> {};

inline constexpr Atom_t Atom;

constexpr bool equal(Atom_t, Atom_t, int&) { return true; }

inline void print(std::ostream& s, Atom_t, int&) { s << "Atom"; }

struct Quote_t : Pie<Quote_t> {
    constexpr Quote_t(const char* symbol) : symbol_{symbol} {}
    const char* symbol_;
};

constexpr int strcmp1(const char* lhs, const char* rhs) {
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

constexpr bool equal(const Quote_t lhs, const Quote_t rhs, int&) { return strcmp1(lhs.symbol_, rhs.symbol_) == 0; }

inline void print(std::ostream& s, const Quote_t atom, int&) { s << '\'' << atom.symbol_; }

constexpr Quote_t quote(const char* symbol) { return Quote_t(symbol); }

constexpr bool isalpha1(int c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }

constexpr bool IsA1(const Quote_t atom, Atom_t, int&) {
    if (*atom.symbol_ == '\0') {
        return false;
    }
    auto c = atom.symbol_;
    do {
        if (!isalpha1(*c) && *c != '-') {
            return false;
        }
        ++c;
    } while (*c != '\0');
    return true;
}

constexpr bool IsAType1(Atom_t, int&) { return true; }

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

constexpr synth_result_t<Atom_t> synth1(Atom_t, int&) { return U; }

template <>
struct synth_result<Quote_t> {
    using type = Atom_t;
};

constexpr synth_result_t<Quote_t> synth1(Quote_t, int&) { return Atom; }
