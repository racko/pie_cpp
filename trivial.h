#pragma once

#include "pie_base.h"
#include <ostream>

struct Trivial_t : Pie<Trivial_t> {};

template <>
struct Height<Trivial_t> : std::integral_constant<int, 0> {};

inline constexpr Trivial_t Trivial;

template <>
struct Equal<Trivial_t, Trivial_t> : std::true_type {};

inline void print(std::ostream& s, Trivial_t) { s << "Trivial"; }

struct Sole_t : Pie<Sole_t> {};

template <>
struct Height<Sole_t> : std::integral_constant<int, 0> {};

template <>
struct Equal<Sole_t, Sole_t> : std::true_type {};

inline void print(std::ostream& s, Sole_t) { s << "sole"; }

inline constexpr Sole_t sole;

constexpr bool IsA1(Sole_t, Trivial_t) { return true; }

constexpr bool IsAType1(Trivial_t) { return true; }

template <>
struct is_normal<Trivial_t> : std::true_type {};

template <>
struct is_value<Trivial_t> : std::true_type {};

template <>
struct is_normal<Sole_t> : std::true_type {};

template <>
struct is_value<Sole_t> : std::true_type {};

template <>
struct synth_result<Trivial_t> {
    using type = U_t;
};

constexpr synth_result_t<Trivial_t> synth1(Trivial_t) { return U; }

template <>
struct synth_result<Sole_t> {
    using type = Trivial_t;
};

constexpr synth_result_t<Sole_t> synth1(Sole_t) { return Trivial; }

// TODO: If e is a Trivial, then e is the same as sole
