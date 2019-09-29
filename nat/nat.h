#pragma once

#include "pie_base.h"
#include <ostream>
#include <type_traits>

struct Nat_t : Pie<Nat_t> {};

template <>
struct Height<Nat_t> : std::integral_constant<int, 0> {};

inline constexpr Nat_t Nat;

template <>
struct Equal<Nat_t, Nat_t> : std::true_type {};

template <>
struct Printer<Nat_t> {
    static void print(std::ostream& s) { s << "Nat"; }
};

constexpr bool IsAType1(Nat_t) { return true; }

template <>
struct is_normal<Nat_t> : std::true_type {};

template <>
struct is_value<Nat_t> : std::true_type {};

template <>
struct synth_result<Nat_t> {
    using type = U_t;
};

constexpr synth_result_t<Nat_t> synth1(Nat_t) { return U; }
