#pragma once

#include "pie_base.h"
#include <ostream>

struct Absurd_t : Pie<Absurd_t> {};

template <>
struct Height<Absurd_t> : std::integral_constant<int, 0> {};

inline constexpr Absurd_t Absurd;

template <>
struct Equal<Absurd_t, Absurd_t> : std::true_type {};

template <>
struct Printer<Absurd_t> {
    static void print(std::ostream& s) { s << "Absurd"; }
};

constexpr bool IsAType1(Absurd_t) { return true; }

template <>
struct is_normal<Absurd_t> : std::true_type {};

template <>
struct is_value<Absurd_t> : std::true_type {};

template <>
struct synth_result<Absurd_t> {
    using type = U_t;
};

constexpr synth_result_t<Absurd_t> synth1(Absurd_t) { return U; }

// TODO: Every expression of type Absurd is neutral, and all of them are the same.
