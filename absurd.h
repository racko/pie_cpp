#pragma once

#include "pie_base.h"
#include <ostream>

struct Absurd_t : Pie<Absurd_t> {
    int height_{};
};

inline constexpr Absurd_t Absurd;

constexpr bool equal(Absurd_t, Absurd_t) { return true; }

inline void print(std::ostream& s, Absurd_t) { s << "Absurd"; }

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
