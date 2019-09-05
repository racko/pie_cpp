#pragma once

#include "pie_base.h"
#include <cwchar>
#include <locale>
#include <ostream>

struct Atom_t : Pie<Atom_t> {};

inline constexpr Atom_t Atom;

bool operator==(Atom_t, Atom_t) { return true; }

std::ostream& operator<<(std::ostream& s, Atom_t) { return s << "Atom"; }

struct Quote_t : Pie<Quote_t> {
    Quote_t(const wchar_t* symbol) : symbol_{symbol} {}
    const wchar_t* symbol_;
};

bool operator==(const Quote_t lhs, const Quote_t rhs) { return std::wcscmp(lhs.symbol_, rhs.symbol_) == 0; }

std::ostream& operator<<(std::ostream& s, const Quote_t atom) { return s << '\'' << atom.symbol_; }

Quote_t quote(const wchar_t* symbol) { return Quote_t(symbol); }

bool IsA_For_Values(const Quote_t atom, Atom_t) {
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

bool IsTheSameAs_For_Values(Atom_t, const Quote_t lhs, const Quote_t rhs) { return lhs == rhs; }

bool IsAType_For_Values(Atom_t) { return true; }

bool AreTheSameType_For_Values(Atom_t, Atom_t) { return true; }

bool IsANormal(const Quote_t atom, Atom_t) {
    return IsA(atom, Atom);
}

bool IsANormalType(Atom_t) {
    return true;
}

bool IsNormalFormOfType(Atom_t, Atom_t) {
    return true;
}

bool IsAValue(Atom_t, const Quote_t atom) {
    return IsA(atom, Atom);
}

Atom_t ComputeValue(Atom_t) {
    return Atom;
}

Quote_t ComputeValue(const Quote_t value) {
    return value;
}

Atom_t Normalize(Atom_t) {
    return Atom;
}

Quote_t Normalize(const Quote_t value) {
    return value;
}
