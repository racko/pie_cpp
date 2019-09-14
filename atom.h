#pragma once

#include "pie_base.h"
#include <cwchar>
#include <locale>
#include <ostream>

struct Atom_t : Pie<Atom_t> {};

inline constexpr Atom_t Atom;

constexpr bool operator==(Atom_t, Atom_t) { return true; }

inline std::wostream& operator<<(std::wostream& s, Atom_t) { return s << "Atom"; }

struct Quote_t : Pie<Quote_t> {
    constexpr Quote_t(const wchar_t* symbol) : symbol_{symbol} {}
    const wchar_t* symbol_;
};

inline bool operator==(const Quote_t lhs, const Quote_t rhs) { return std::wcscmp(lhs.symbol_, rhs.symbol_) == 0; }

inline std::wostream& operator<<(std::wostream& s, const Quote_t atom) { return s << '\'' << atom.symbol_; }

constexpr Quote_t quote(const wchar_t* symbol) { return Quote_t(symbol); }

inline bool IsA(const Quote_t atom, Atom_t) {
    const std::locale loc("en_US.UTF8");
    if (*atom.symbol_ == '\0') {
        return false;
    }
    auto c = atom.symbol_;
    do {
        if (!std::isalpha(*c, loc) && *c != '-') {
            return false;
        }
        ++c;
    } while (*c != '\0');
    return true;
}

constexpr bool IsAType(Atom_t) { return true; }

template<>
struct is_normal<Atom_t> : std::true_type {};

template<>
struct is_value<Atom_t> : std::true_type {};

template<>
struct is_normal<Quote_t> : std::true_type {};

template<>
struct is_value<Quote_t> : std::true_type {};

template <>
struct synth_result<Atom_t> {
    using type = U_t;
};

constexpr synth_result_t<Atom_t> synth(Atom_t) { return U; }

template <>
struct synth_result<Quote_t> {
    using type = Atom_t;
};

constexpr synth_result_t<Quote_t> synth(Quote_t) { return Atom; }
