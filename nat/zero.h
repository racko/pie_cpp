#pragma once

#include "nat/nat.h"
#include "pie_base.h"
#include <ostream>
#include <utility>

struct Zero_t : Pie<Zero_t> {
};

template <>
struct Height<Zero_t> : std::integral_constant<int, 0> {};

inline constexpr Zero_t zero;

constexpr bool equal(Zero_t, Zero_t) { return true; }

inline void print(std::ostream& s, Zero_t) { s << "zero"; }

constexpr bool IsA1(Zero_t, Nat_t) { return true; }

template <>
struct is_normal<Zero_t> : std::true_type {};

template <>
struct is_value<Zero_t> : std::true_type {};

template <>
struct synth_result<Zero_t> {
    using type = Nat_t;
};

constexpr synth_result_t<Zero_t> synth1(Zero_t) { return Nat; }
